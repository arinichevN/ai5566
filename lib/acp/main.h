#ifndef ACP_MAIN_H
#define ACP_MAIN_H

/*
 * 
 * Application Communication Protocol
 * 
 */

#include "../../lib/check.h"
#include "../../lib/common.h"
#include "../../lib/ton.h"

#define ACP_BUF_MAX_LENGTH			64
#define ACP_CRC_BUF_MAX_LENGTH		2
#define ACP_MIN_PACK_LENGTH			7
#define ACP_CRC_INI					33
#define ACP_CRC_EXTRA				11
#define ACP_FLOAT_BUF_LEN			12
#define ACP_INT_BUF_LEN				12

#define ACP_CHECK_CRC				1

#define ACP_DELIMITER_START			'<'
#define ACP_DELIMITER_COLUMN		';'
#define ACP_DELIMITER_END			'>'

#define ACP_DELIMITER_START_STR		"<"
#define ACP_DELIMITER_COLUMN_STR	";"
#define ACP_DELIMITER_END_STR		">"

#define ACP_SIGN_REQUEST_GET				'?' //response required
#define ACP_SIGN_REQUEST_SET				'&' //no response required 
#define ACP_SIGN_REQUEST_GET_BROADCAST		'*' //for everyone in the net, response required
#define ACP_SIGN_REQUEST_SET_BROADCAST		'#' //for everyone in the net, no response required
#define ACP_SIGN_RESPONSE					'!' //response to request

#define ADSS ACP_DELIMITER_START_STR
#define ADCS ACP_DELIMITER_COLUMN_STR 
#define ADES ACP_DELIMITER_END_STR

#define ACP_IND_SIGN				1

#define ACP_REQUEST_IND_COMMAND		1
#define ACP_REQUEST_IND_ID			2
#define ACP_REQUEST_IND_PARAM1		3
#define ACP_REQUEST_IND_PARAM2		4
#define ACP_REQUEST_IND_PARAM3		5
#define ACP_REQUEST_IND_PARAM4		6

#define ACP_RESPONSE_IND_ID			1
#define ACP_RESPONSE_IND_PARAM1		2
#define ACP_RESPONSE_IND_PARAM2		3
#define ACP_RESPONSE_IND_PARAM3		4
#define ACP_RESPONSE_IND_PARAM4		5

enum ACPStateE {
	ACP_DONE,
	ACP_BUSY,
	ACP_WAIT,
	ACP_SUCCESS,
	ACP_NOT_FOUND,
	
	ACP_INIT,
	ACP_READ_REQUEST,
	ACP_CONSIDER_PACK,
	ACP_CONSIDER_REQUEST,
	ACP_CONSIDER_RESPONSE,
	ACP_READ_RESPONSE,
	ACP_SEND_RESPONSE,
	ACP_SEND_REQUEST,
	ACP_IDLE, 
	
	ACP_ERROR,
	ACP_ERROR_BAD_CHANNEL_ID,
	ACP_ERROR_WAIT_RESPONSE_TIMEOUT,
	ACP_ERROR_PACK_TIMEOUT,
	ACP_ERROR_BUFFER_OVERFLOW,
	ACP_ERROR_NO_START_DELIMITER,
	ACP_ERROR_SHORT_PACKAGE,
	ACP_ERROR_FORMAT,
	ACP_ERROR_WRITE,
	ACP_ERROR_CRC,
	ACP_ERROR_SIGN,
	ACP_ERROR_BUSY,
	ACP_ERROR_QUEUE,
	ACP_ERROR_ACPL_IS_NULL
};


extern uint8_t acp_calcStrCRC(const char *str);
extern uint8_t acp_getPackCRC(const char *pack_str);
extern void acp_addCRC(char *str);

extern const char *acp_getStateStr(int v);
extern int acp_packGetCellS (const char *pack_str, int cell_ind, char *out, size_t len);
extern int acp_packGetCellI (const char *pack_str, int cell_ind, int *out);
extern int acp_packGetCellF (const char *pack_str, int cell_ind, double *out);
extern int acp_packGetFts(const char *pack_str, int channel_id, Fts *out);

extern int acp_buildPackS(char *buf, size_t buf_max_len, char sign, const char *v1);
extern int acp_buildPackSI(char *buf, size_t buf_max_len, char sign, const char *v1, int v2);
extern int acp_buildPackSF(char *buf, size_t buf_max_len, char sign, const char *v1, double v2);
extern int acp_buildPackI(char *buf, size_t buf_max_len, char sign, int v);
extern int acp_buildPackAi(char *buf, size_t buf_max_len, char sign, const int *arr, size_t arr_len);
extern int acp_buildPackUl(char *buf, size_t buf_max_len, char sign, unsigned long v);
extern int acp_buildPackF(char *buf, size_t buf_max_len, char sign, double v);
extern int acp_buildPackIS(char *buf, size_t buf_max_len, char sign, int v1, const char *v2);
extern int acp_buildPackII(char *buf, size_t buf_max_len, char sign, int v1, int v2);
extern int acp_buildPackIUl(char *buf, size_t buf_max_len, char sign, int v1, unsigned long v2);
extern int acp_buildPackIIUl(char *buf, size_t buf_max_len, char sign, int v1, int v2, unsigned long v3);
extern int acp_buildPackIIUlI(char *buf, size_t buf_max_len, char sign, int v1, int v2, unsigned long v3, int v4);
extern int acp_buildPackIF(char *buf, size_t buf_max_len, char sign, int v1, double v2);
extern int acp_buildPackIIF(char *buf, size_t buf_max_len, char sign, int v1, int v2, double v3);
extern int acp_buildPackIFts(char *buf, size_t buf_max_len, char sign, int v1, Fts *v2);
extern int acp_buildPackIUlI(char *buf, size_t buf_max_len, char sign, int v1, unsigned long v2, int v3);
extern int acp_buildPackIII(char *buf, size_t buf_max_len, char sign, int v1, int v2, int v3);
extern int acp_buildPackIIII(char *buf, size_t buf_max_len, char sign, int v1, int v2, int v3, int v4);
extern int acp_buildPackIIFI(char *buf, size_t buf_max_len, char sign, int v1, int v2, double v3, int v4);
extern int acp_buildPack5I(char *buf, size_t buf_max_len, char sign, int v1, int v2, int v3, int v4, int v5);
extern int acp_buildPackIFI(char *buf, size_t buf_max_len, char sign, int v1, double v2, int v3);
extern int acp_buildPackSII(char *buf, size_t buf_max_len, char sign, const char *v1, int v2, int v3);
extern int acp_buildPackSIII(char *buf, size_t buf_max_len, char sign, const char *v1, int v2, int v3, int v4);
extern int acp_buildPackSIUl(char *buf, size_t buf_max_len, char sign, const char *v1, int v2, unsigned long v3);
extern int acp_buildPackSIF(char *buf, size_t buf_max_len, char sign, const char *v1, int v2, double v3);
	
#endif 
