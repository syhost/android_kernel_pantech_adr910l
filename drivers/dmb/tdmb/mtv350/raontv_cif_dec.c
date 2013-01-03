/******************************************************************************** 
* (c) COPYRIGHT 2010 RAONTECH, Inc. ALL RIGHTS RESERVED.
* 
* This software is the property of RAONTECH and is furnished under license by RAONTECH.                
* This software may be used only in accordance with the terms of said license.                         
* This copyright noitce may not be remoced, modified or obliterated without the prior                  
* written permission of RAONTECH, Inc.                                                                 
*                                                                                                      
* This software may not be copied, transmitted, provided to or otherwise made available                
* to any other person, company, corporation or other entity except as specified in the                 
* terms of said license.                                                                               
*                                                                                                      
* No right, title, ownership or other interest in the software is hereby granted or transferred.       
*                                                                                                      
* The information contained herein is subject to change without notice and should 
* not be construed as a commitment by RAONTECH, Inc.                                                                    
* 
* TITLE 	  : RAONTECH TV CIF Decoder source file for T-DMB and DAB. 
*
* FILENAME    : raontv_cif_dec.c
*
* DESCRIPTION : 
*		Library of routines to initialize, and operate on, the RAONTECH T-DMB demod.
*
********************************************************************************/

/******************************************************************************** 
* REVISION HISTORY
*
*    DATE	  	  NAME				REMARKS
* ----------  -------------    --------------------------------------------------
* 09/27/2010  Ko, Kevin        Created.
********************************************************************************/

#include <linux/kernel.h>
#include <linux/string.h>
#include "mtv350_bb.h"

#include "raontv.h"


#ifdef RTV_CIF_HEADER_INSERTED


#if defined(__KERNEL__) && defined(RTV_CIF_LINUX_USER_SPACE_COPY_USED)
	#include <asm/uaccess.h>

	#define CIF_DATA_COPY(dst_ptr, src_ptr, size)									\
		do																		\
		{																		\
			int ret;																\
			if((ret=copy_to_user((void __user *)dst_ptr, (const void *)src_ptr, size)) < 0) \
				RTV_DBGMSG1("[CIF Parser] copy_to_user error: %d\n", ret); 			\
		} while(0)
#else
	#define CIF_DATA_COPY(dst_ptr, src_ptr, size) 	memcpy(dst_ptr, src_ptr, size); // copy data to destination buffer.

#endif //0928 build error

#define MAX_SUBCH_SIZE_PER_CIF		(16 * 188)


#define SUBCH_GRP_IDX(sub_ch_id)		(sub_ch_id >> 5) // Divide by 32
#define MOD32(x)                    	(x & 31)
static U32 g_aAddedSubChIdBits[2]; /* Used sub channel ID bits. [0]: 0 ~ 31, [1]: 32 ~ 63 */


#if defined(RTV_IF_MPEG2_SERIAL_TSIF) || defined(RTV_IF_SPI_SLAVE) || defined(RTV_IF_QUALCOMM_TSIF)
typedef enum
{
	PARSER_STATUS__OK = 0,
	PARSER_STATUS__SHORT_HDR_0,
	PARSER_STATUS__SHORT_HDR_1,	
	PARSER_STATUS__SHORT_PKT,
	PARSER_STATUS__INVALID_SC_0,
	PARSER_STATUS__INVALID_SC_1,
	PARSER_STATUS__INVALID_SC_2,
	PARSER_STATUS__INVALID_HDR,
	PARSER_STATUS__UNKNOWN_ERR
} PARSER_STATUS_TYPE;

typedef struct
{
	UINT tsi_counter;
	UINT cif_size; // (FIC+MSC0+MSC1 SIZE)	
	UINT pkt_size; // (TSI_CIF_MODE_HEADER + cif_size + padding_bytes)
	
	UINT fic_size;

	UINT subch_id[RTV_MAX_NUM_SUB_CHANNEL_USED];
	UINT subch_size[RTV_MAX_NUM_SUB_CHANNEL_USED];

	UINT padding_bytes;
} TSI_CIF_HEADER_INFO;


static U8 temp_ts_buf_pool[2 * (RTV_CIF_HEADER_SIZE + 384 + MAX_SUBCH_SIZE_PER_CIF + 188/*padding*/)];
static U8 *g_pbPrevBufAllocPtr; // allocation pointer.(Fixed) Used to free.

static U8 *g_pbPrevTsBuf; // 0xe2e2 start pointer.
static UINT g_nPrevTsLen;


#define _DEBUG_CIF_PARSER


#ifdef _DEBUG_CIF_PARSER
static void parser_status_msg(PARSER_STATUS_TYPE status)
{
	switch( status )
	{
		case PARSER_STATUS__SHORT_HDR_0: RTV_DBGMSG0("[CIF Parser] PARSER_STATUS__SHORT_HDR_0\n"); break;
		case PARSER_STATUS__SHORT_HDR_1: RTV_DBGMSG0("[CIF Parser] PARSER_STATUS__SHORT_HDR_1\n"); break;
		case PARSER_STATUS__INVALID_SC_0: RTV_DBGMSG0("[CIF Parser] PARSER_STATUS__INVALID_SC_0\n"); break;
		case PARSER_STATUS__INVALID_SC_1: RTV_DBGMSG0("[CIF Parser] PARSER_STATUS__INVALID_SC_1\n"); break;
		case PARSER_STATUS__INVALID_SC_2: RTV_DBGMSG0("[CIF Parser] PARSER_STATUS__INVALID_SC_2\n"); break;
		default: 
			RTV_DBGMSG0("[CIF Parser] PARSER_STATUS__UNKNOWN_ERR\n"); 
			break;
	}
}
#define PARSER_STATUS_MSG(status)		parser_status_msg(status)
#else
#define PARSER_STATUS_MSG(status)		((void)0) 
#endif


static INLINE BOOL verify_header(TSI_CIF_HEADER_INFO *hdr_info)
{
	UINT i;

	if((hdr_info->fic_size != 0) && (hdr_info->fic_size != 384))
		return FALSE;

	if(hdr_info->cif_size > (384 + MAX_SUBCH_SIZE_PER_CIF)/* FIC+MSC0+MSC1 SIZE */)
		return FALSE;

	for(i=0; i<RTV_MAX_NUM_SUB_CHANNEL_USED; i++)
	{
		if(hdr_info->subch_size[i] > MAX_SUBCH_SIZE_PER_CIF) 
			return FALSE;

		if(hdr_info->subch_size[i] != 0) // 
		{
			if((g_aAddedSubChIdBits[SUBCH_GRP_IDX(hdr_info->subch_id[i])] & (1<<MOD32(hdr_info->subch_id[i]))) == 0)
			{
				return FALSE; // The decoded subch id was not registered.
			}
		}
	}

	return TRUE;
}


static INLINE void parsing_header(TSI_CIF_HEADER_INFO *hdr_info, const U8 *hdr_buf_ptr)
{
	hdr_info->tsi_counter = (hdr_buf_ptr[2]<<8) | hdr_buf_ptr[3]; // TSI COUNTER (CIF UNIT)
	hdr_info->cif_size    = ((hdr_buf_ptr[4]<<8) | hdr_buf_ptr[5]); // CIF SIZE (FIC+MSC0+MSC1 SIZE)	
	hdr_info->fic_size    = ((hdr_buf_ptr[6]&0x03)<<8) | hdr_buf_ptr[7];

	hdr_info->subch_id[0]   = hdr_buf_ptr[8] >> 2;
	hdr_info->subch_size[0] = (((hdr_buf_ptr[8]&0x03)<<8) | hdr_buf_ptr[9]) * 4;	

#if (RTV_MAX_NUM_SUB_CHANNEL_USED >= 2)
	hdr_info->subch_id[1]   = hdr_buf_ptr[10] >> 2;
	hdr_info->subch_size[1] = (((hdr_buf_ptr[10]&0x03)<<8) | hdr_buf_ptr[11]) * 4;	
#endif

#if (RTV_MAX_NUM_SUB_CHANNEL_USED >= 3)
	hdr_info->subch_id[2]   = hdr_buf_ptr[12] >> 2;
	hdr_info->subch_size[2] = (((hdr_buf_ptr[12]&0x03)<<8) | hdr_buf_ptr[13]) * 4;	
#endif

#if (RTV_MAX_NUM_SUB_CHANNEL_USED == 4)
	hdr_info->subch_id[3]   = hdr_buf_ptr[14] >> 2;
	hdr_info->subch_size[3] = (((hdr_buf_ptr[14]&0x03)<<8) | hdr_buf_ptr[15]) * 4;	
#endif	

#if (RTV_MAX_NUM_SUB_CHANNEL_USED > 4)
	#error "Code not present"
#endif
}


static PARSER_STATUS_TYPE get_header(TSI_CIF_HEADER_INFO *hdr_info, 
									 const U8 *prev_ts_buf_ptr, UINT prev_ts_len,
									 const U8 *cur_ts_buf_ptr, UINT cur_ts_len)
{
	UINT mod, i, k, diff;
	U8 header_data[RTV_CIF_HEADER_SIZE];

	if((prev_ts_len + cur_ts_len) < RTV_CIF_HEADER_SIZE)
		return PARSER_STATUS__SHORT_HDR_0; /* To processing in next time. */

	if(prev_ts_len == 0)
	{
		if((cur_ts_buf_ptr[0] != 0xE2) || (cur_ts_buf_ptr[1] != 0xE2))
			return PARSER_STATUS__INVALID_SC_0;

		parsing_header(hdr_info, cur_ts_buf_ptr);
	}
	else
	{
		if(prev_ts_len >= RTV_CIF_HEADER_SIZE)
		{
			if((prev_ts_buf_ptr[0] != 0xE2) || (prev_ts_buf_ptr[1] != 0xE2))
				return PARSER_STATUS__INVALID_SC_1;

			parsing_header(hdr_info, prev_ts_buf_ptr);
		}
		else
		{ 
			diff = RTV_CIF_HEADER_SIZE - prev_ts_len;

			for(i=0; i < prev_ts_len; i++)
				header_data[i] = prev_ts_buf_ptr[i];

			for(k=0; k<diff; i++, k++)
				header_data[i] = cur_ts_buf_ptr[k];

			if((header_data[0] != 0xE2) || (header_data[1] != 0xE2))
				return PARSER_STATUS__INVALID_SC_2;

			parsing_header(hdr_info, header_data);
		}
	}

	if(verify_header(hdr_info) == FALSE)
		return PARSER_STATUS__INVALID_HDR;

	/* Calculates a TSI CIF packet size. */
	hdr_info->pkt_size = RTV_CIF_HEADER_SIZE + hdr_info->cif_size; 
	hdr_info->padding_bytes = 0;
	
	mod = hdr_info->pkt_size % 188;
	if(mod != 0)
	{
		hdr_info->pkt_size      += (188 - mod); // next 188 align
		hdr_info->padding_bytes = (188 - mod);  // next 188 align
	}

	return PARSER_STATUS__OK;
}


static void pull_packet(const U8 **cur_ts_buf_ptr, UINT *cur_ts_len, UINT size)
{
	UINT diff;

	if(g_nPrevTsLen == 0)
	{
		*cur_ts_buf_ptr += size;
		*cur_ts_len     -= size;
	}
	else
	{
		if(g_nPrevTsLen >= size)
		{
			g_pbPrevTsBuf += size;
			g_nPrevTsLen  -= size;
		}
		else
		{
			diff = size - g_nPrevTsLen;

			*cur_ts_buf_ptr += diff;
			*cur_ts_len     -= diff;

			g_pbPrevTsBuf    = NULL;
			g_nPrevTsLen = 0;
		}
	}
}


static BOOL copy_data(U8 *out_buf, const U8 **cur_buf_ptr, UINT *cur_ts_len,
			          UINT req_size)
{
	UINT copy_bytes;

	if(g_nPrevTsLen != 0) // pure data. removed header.
	{
		copy_bytes = MIN(g_nPrevTsLen, req_size);
		
		CIF_DATA_COPY(out_buf, g_pbPrevTsBuf, copy_bytes); // copy data to destination buffer.
		out_buf += copy_bytes;

		/* Update buffer pointer and size. */
		g_pbPrevTsBuf += copy_bytes;
		g_nPrevTsLen  -= copy_bytes;

		req_size -= copy_bytes;
		if(req_size == 0)
			return TRUE; // finished
	}

	if(*cur_ts_len < req_size)
	{
		RTV_DBGMSG0("[copy_data] Abnormal state. (cur_ts_len < req_size)\n");
		return FALSE;
	}

	copy_bytes = MIN(*cur_ts_len, req_size);
	CIF_DATA_COPY(out_buf, *cur_buf_ptr, copy_bytes);

	*cur_buf_ptr += copy_bytes;
	*cur_ts_len  -= copy_bytes;

	req_size -= copy_bytes;
	if(req_size == 0)
		return TRUE;
	else
	{
		RTV_DBGMSG0("[copy_data] Abnormal state. (req_size != 0)\n");
		return FALSE;
	}	
}


static void collect_ts_data(const U8 *cur_ts_buf_ptr, UINT cur_ts_len)
{
	if(g_nPrevTsLen == 0)
	{
		/* Set the pointer of previous ts buffer to the base pointer of allocation buffer. */
		g_pbPrevTsBuf = g_pbPrevBufAllocPtr; 
	}

	if(cur_ts_len != 0)
	{	
		/* Save for next processing. */
		memcpy(g_pbPrevTsBuf+g_nPrevTsLen, cur_ts_buf_ptr, cur_ts_len); 
		g_nPrevTsLen += cur_ts_len;
	}
}


static PARSER_STATUS_TYPE search_start_cmd(TSI_CIF_HEADER_INFO *hdr_info, const U8 **cur_ts_buf_ptr, UINT *cur_ts_len)
{
	UINT diff;
	UINT which_buf; /* 0: prev, 1: cur */
	U8 next_start_cmd_data[2];
	PARSER_STATUS_TYPE status = PARSER_STATUS__SHORT_HDR_1;
	const U8 *temp_cur_ts_buf_ptr = *cur_ts_buf_ptr;
	UINT temp_cur_ts_len = *cur_ts_len;

	while((g_nPrevTsLen + temp_cur_ts_len) >= RTV_CIF_HEADER_SIZE)
	{
		if(g_nPrevTsLen != 0) which_buf = 0; /* Previous buffer used in this time. */
		else				  which_buf = 1; /* Current buffer used in this time. */
		
		status = get_header(hdr_info, g_pbPrevTsBuf, g_nPrevTsLen, temp_cur_ts_buf_ptr, temp_cur_ts_len);
		if(status == PARSER_STATUS__OK)
		{
			/* Check the next START CMD field. */
			if((g_nPrevTsLen + temp_cur_ts_len) < (hdr_info->pkt_size + 2)) // next start cmd
			{	
				status = PARSER_STATUS__SHORT_PKT; 
				break; /* To processing in next time. */
			}	

			if(g_nPrevTsLen == 0)
			{
				next_start_cmd_data[0] = temp_cur_ts_buf_ptr[hdr_info->pkt_size];
				next_start_cmd_data[1] = temp_cur_ts_buf_ptr[hdr_info->pkt_size + 1];
			}
			else
			{
				if(g_nPrevTsLen >= (hdr_info->pkt_size + 2))
				{	/* In case, all start command bytes plac in the previous buffer. */
					next_start_cmd_data[0] = g_pbPrevTsBuf[hdr_info->pkt_size];
					next_start_cmd_data[1] = g_pbPrevTsBuf[hdr_info->pkt_size + 1];
				}
				else
				{
					diff = hdr_info->pkt_size - g_nPrevTsLen;
					next_start_cmd_data[0] = temp_cur_ts_buf_ptr[diff];
					next_start_cmd_data[1] = temp_cur_ts_buf_ptr[diff + 1];
				}
			}

			if((next_start_cmd_data[0] == 0xE2) && (next_start_cmd_data[1] == 0xE2))
			{
				status = PARSER_STATUS__OK;
				break;
			}
			else
			{	/* This is data. (not START_CMD) "pkt_size" is invalid value! */
				//RTV_DBGMSG2("[next: data(CMD)] 0x%02X, 0x%02X\n", next_start_cmd_data[0], next_start_cmd_data[1]);
				// statistic.invalid_sc_4++;
			}
		}

		if(which_buf == 0)
		{
			if(g_pbPrevTsBuf != 0)
			{
				g_nPrevTsLen--;
				g_pbPrevTsBuf++;
			}
			else
				g_pbPrevTsBuf = NULL;
		}
		else
		{
			if(temp_cur_ts_len != 0)
			{
				temp_cur_ts_len--;
				temp_cur_ts_buf_ptr++;
			}
			else
				temp_cur_ts_buf_ptr = NULL;
		}
	} /* end of while((g_nPrevTsLen + temp_cur_ts_len) >= RTV_CIF_HEADER_SIZE) */

	if(temp_cur_ts_len != *cur_ts_len)
	{
		/* Update the current variables to chagne the pointer and length for the caller. */
		*cur_ts_buf_ptr = temp_cur_ts_buf_ptr;
		*cur_ts_len     = temp_cur_ts_len;
	}
	
	return status; /* To processing in next time. */
}


void rtvCIFDEC_Decode(RTV_CIF_DEC_INFO *ptDecInfo, const U8 *pbTsBuf, UINT nTsLen)
{
	PARSER_STATUS_TYPE status; 
	TSI_CIF_HEADER_INFO hdr_info;
	UINT i=0;
	U8 *dest_msc_buf_ptr[RTV_MAX_NUM_SUB_CHANNEL_USED];
	const U8 *cur_ts_buf_ptr = pbTsBuf;

	ptDecInfo->fic_size = 0;

	for(i=0; i<RTV_MAX_NUM_SUB_CHANNEL_USED; i++) 
	{
		ptDecInfo->msc_size[i] = 0;
		dest_msc_buf_ptr[i]         = ptDecInfo->msc_buf_ptr[i];
	}

	if(nTsLen == 0)
		return;
	
	/* Parsing the TSI CIF data by processing a TSI CIF packet unit. */
	do
	{
		status = get_header(&hdr_info, g_pbPrevTsBuf, g_nPrevTsLen, cur_ts_buf_ptr, nTsLen);
		if(status != PARSER_STATUS__OK)
		{
			PARSER_STATUS_MSG(status);

			/* hdr_info need to update if search ok. */
			status = search_start_cmd(&hdr_info, &cur_ts_buf_ptr, &nTsLen);
			if(status != PARSER_STATUS__OK)
			{
				PARSER_STATUS_MSG(status);
				collect_ts_data(cur_ts_buf_ptr, nTsLen);
				return; /* To processing in next time. */
			}
		}

		/* Check if we received a full CIF packet including the previous TS data or not. */
		if((g_nPrevTsLen + nTsLen) < hdr_info.pkt_size)
		{	
			collect_ts_data(cur_ts_buf_ptr, nTsLen);
			//statistic.short_packet_size_0++;
			return; /* To processing in next time. */
		}

		RTV_DBGMSG2("TSI CIF SIZE(FIC+MSC0+MSC1 SIZE): %d, TSI_COUNTER: %d\n", hdr_info.cif_size, hdr_info.tsi_counter);
		RTV_DBGMSG3("pkt_size(HEADER+FIC+MSC0+MSC1 SIZE): %d(%d*188), padding_bytes: %d\n", hdr_info.pkt_size, hdr_info.pkt_size/188, hdr_info.padding_bytes);

	    /* Process a full CIF TS packet. */

		/* Remove the header. */
		pull_packet(&cur_ts_buf_ptr, &nTsLen, RTV_CIF_HEADER_SIZE);

		/* Get and process FIC data. */
		if(hdr_info.fic_size != 0)
		{
			copy_data(ptDecInfo->fic_buf_ptr, &cur_ts_buf_ptr, &nTsLen, hdr_info.fic_size);

			ptDecInfo->fic_size = hdr_info.fic_size;
		}
			
		/* Get and process MSC data. */
		for(i=0; i<RTV_MAX_NUM_SUB_CHANNEL_USED; i++) // SUBCHx ID
		{		
			if(hdr_info.subch_size[i] != 0) 
			{
				ptDecInfo->msc_subch_id[i] = hdr_info.subch_id[i];
				
				copy_data(dest_msc_buf_ptr[i], &cur_ts_buf_ptr, &nTsLen, hdr_info.subch_size[i]);
				
				dest_msc_buf_ptr[i]    += hdr_info.subch_size[i];
				ptDecInfo->msc_size[i] += hdr_info.subch_size[i];				
			}
		}

		/* Remove the padding. */
		pull_packet(&cur_ts_buf_ptr, &nTsLen, hdr_info.padding_bytes);

	} while((nTsLen != 0) || (g_nPrevTsLen != 0)); /* To collect the remaind data, we check 0 bytes. */
}

#else
/* SPI */
static UINT g_nUsedOutDecBufIdxBits; /* 0 ~ (RTV_MAX_NUM_SUB_CHANNEL_USED-1) */
static UINT g_aOutDecBufIdx[64];

/* Only MSC0 data. */
void rtvCIFDEC_Decode(RTV_CIF_DEC_INFO *ptDecInfo, const U8 *pbTsBuf, UINT nTsLen)
{     
	UINT i;
	UINT ch_length, subch_id, ch_info, subch_size;
	const U8 *data_ptr;
	const U8 *cif_header_ptr = pbTsBuf;
	U8 *dest_msc_buf_ptr[RTV_MAX_NUM_SUB_CHANNEL_USED-1]; // MSC0 only. Except MSC1.
	UINT nOutDecBufIdx = 0;

	for(i=0; i<RTV_MAX_NUM_SUB_CHANNEL_USED-1; i++) /* Only MSC0. */
	{
		ptDecInfo->msc_size[i] = 0;
		dest_msc_buf_ptr[i]    = ptDecInfo->msc_buf_ptr[i];
	}

	if(nTsLen == 0)
		return;
		
//	RTV_DBGMSG1("\n[rtvCIFDEC_Decode] msc_total: %d\n", nTsLen);
//	printk("[nTsLen: %u] [0x%02X], [0x%02X], [0x%02X], [0x%02X], [0x%02X]\n", nTsLen, pbTsBuf[0], pbTsBuf[1], pbTsBuf[2], pbTsBuf[3], pbTsBuf[3]);			
	
	do
	{
		subch_id  = cif_header_ptr[0] >> 2;
		ch_info   = ((cif_header_ptr[0] & 0x03) << 1) | ((cif_header_ptr[1] & 0x80)>>7);
		ch_length = (cif_header_ptr[2]<<8) | cif_header_ptr[3];

		if(ch_length < RTV_CIF_HEADER_SIZE)
		{
			RTV_DBGMSG0("[rtvCIFDEC_Decode] Invalid CIF Header length. May be broken...\n"); 
			return;
		}

		subch_size = ch_length - RTV_CIF_HEADER_SIZE;
		data_ptr   = cif_header_ptr + RTV_CIF_HEADER_SIZE;   

	//	RTV_DBGMSG3("ch_length: %d, subch_id: %d, subch_size: %d\n", ch_length, subch_id, subch_size);

		/* Verify a Header */
		if(subch_size > 3*1024/*MSC0*/) 
		{
			RTV_DBGMSG0("[rtvCIFDEC_Decode] Invalid sub channel SIZE.\n"); 
			return;
		}

		/* Check for this sub channel ID was registerd ? */
		if(subch_size != 0) // 
		{
			if((g_aAddedSubChIdBits[SUBCH_GRP_IDX(subch_id)] & (1<<MOD32(subch_id))) == 0)
			{
				RTV_DBGMSG0("[rtvCIFDEC_Decode] Invalid sub channel ID.\n"); 
				return;
			}
		}
		
		nOutDecBufIdx = g_aOutDecBufIdx[subch_id]; // Out buffer index determined when add sub channel.
      
		ptDecInfo->msc_subch_id[nOutDecBufIdx] = subch_id;		

		CIF_DATA_COPY(dest_msc_buf_ptr[nOutDecBufIdx], data_ptr, subch_size);		
		
		dest_msc_buf_ptr[nOutDecBufIdx]    += subch_size;
		ptDecInfo->msc_size[nOutDecBufIdx] += subch_size;				

//		RTV_DBGMSG3("subch_id: %d, subch_size: %d, ch_info: %d\n", subch_id, subch_size, ch_info);
//		RTV_DBGMSG3("\tsubch_data[0]: 0x%02X, subch_data[1]: 0x%02X, subch_data[2]: 0x%02X\n", data_ptr[0], data_ptr[1], data_ptr[2]);

		cif_header_ptr += ch_length;                   
		nTsLen         -= ch_length;
	} while(nTsLen != 0);
}
#endif

/* 
This function delete a sub channel ID from the CIF decoder. 
This function should called after Sub Channel Close. */
void rtvCIFDEC_DeleteSubChannelID(UINT nSubChID)
{
	if((g_aAddedSubChIdBits[SUBCH_GRP_IDX(nSubChID)] & (1<<MOD32(nSubChID))) == 0)
		return; // not opened! already closed!	
	
	// Delete a sub channel ID.
	g_aAddedSubChIdBits[SUBCH_GRP_IDX(nSubChID)] &= ~(1 << MOD32(nSubChID)); 

#if defined(RTV_IF_SPI)
	g_nUsedOutDecBufIdxBits &= ~(1<<g_aOutDecBufIdx[nSubChID]);
	//g_aOutDecBufIdx[nSubChID] = 0xFF;
#endif
}

/*
This function add a sub channel ID to the CIF decoder to verify CIF header.
This function should called before Sub Channel Open. */
void rtvCIFDEC_AddSubChannelID(UINT nSubChID)
{
#if defined(RTV_IF_SPI)
	UINT nOutDecBufIdx;
#endif

	// Check if already registerd ?
	if( g_aAddedSubChIdBits[SUBCH_GRP_IDX(nSubChID)] & (1<<MOD32(nSubChID)) )
	{
		return; // Already added.
	}

	/* Adds a sub channel ID. */
	g_aAddedSubChIdBits[SUBCH_GRP_IDX(nSubChID)] |= (1 << MOD32(nSubChID)); 

#if defined(RTV_IF_SPI)
	/* Only MSC0. */
	for(nOutDecBufIdx=0; nOutDecBufIdx<RTV_MAX_NUM_SUB_CHANNEL_USED-1; nOutDecBufIdx++) 
	{
		if((g_nUsedOutDecBufIdxBits & (1<<nOutDecBufIdx)) == 0) 		
		{
			break;
		}
	}

	g_nUsedOutDecBufIdxBits |= (1<<nOutDecBufIdx);
	g_aOutDecBufIdx[nSubChID] = nOutDecBufIdx;
#endif
}

/*
This function deinitialize the CIF decoder.
This function should called when Power Off.
*/
void rtvCIFDEC_Deinit(void)
{
#if defined(RTV_IF_MPEG2_SERIAL_TSIF) || defined(RTV_IF_SPI_SLAVE) || defined(RTV_IF_QUALCOMM_TSIF)
	// Free a pool
	g_pbPrevBufAllocPtr = NULL;
#endif	
}


/*
This function initialize the CIF decoder.
This function should called when Power On.
*/
void rtvCIFDEC_Init(void)
{
#if defined(RTV_IF_MPEG2_SERIAL_TSIF) || defined(RTV_IF_SPI_SLAVE) || defined(RTV_IF_QUALCOMM_TSIF)
	/* Allocate a new buffer from memory pool. */
	g_pbPrevBufAllocPtr = &temp_ts_buf_pool[0];		

	g_nPrevTsLen  = 0;		
	g_pbPrevTsBuf = NULL;

#else
	g_nUsedOutDecBufIdxBits = 0x00; 
#endif	

	g_aAddedSubChIdBits[0] = 0;
	g_aAddedSubChIdBits[1] = 0;
}


#endif // #ifdef RTV_CIF_HEADER_INSERTED