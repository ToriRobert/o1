/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-PDU-Contents"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_GNBCUConfigurationUpdate_H_
#define	_GNBCUConfigurationUpdate_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ProtocolIE-Container.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* GNBCUConfigurationUpdate */
typedef struct GNBCUConfigurationUpdate {
	ProtocolIE_Container_4587P9_t	 protocolIEs;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} GNBCUConfigurationUpdate_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_GNBCUConfigurationUpdate;
extern asn_SEQUENCE_specifics_t asn_SPC_GNBCUConfigurationUpdate_specs_1;
extern asn_TYPE_member_t asn_MBR_GNBCUConfigurationUpdate_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _GNBCUConfigurationUpdate_H_ */
#include <asn_internal.h>
