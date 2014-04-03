/* 
   Copyright (C) 2003-2008 FreeIPMI Core Team

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.  
*/

/* $Id: ipmi-pef-and-alerting-cmds-api.h,v 1.3 2008/03/28 00:14:50 chu11 Exp $ */

#ifndef _IPMI_PEF_AND_ALERTING_CMDS_API_H
#define _IPMI_PEF_AND_ALERTING_CMDS_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <freeipmi/api/ipmi-api.h>
#include <freeipmi/fiid/fiid.h>

int8_t ipmi_cmd_get_pef_capabilities (ipmi_ctx_t ctx, 
				      fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_arm_pef_postpone_timer (ipmi_ctx_t ctx, 
					uint8_t pef_postpone_timeout, 
					fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_pef_configuration_parameters_pef_control (ipmi_ctx_t ctx, 
							      uint8_t pef, 
							      uint8_t pef_event_messages, 
							      uint8_t pef_startup_delay, 
							      uint8_t pef_alert_startup_delay, 
							      fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_pef_configuration_parameters_pef_action_global_control (ipmi_ctx_t ctx, 
									    uint8_t alert_action,
									    uint8_t power_down_action,
									    uint8_t reset_action,
									    uint8_t power_cycle_action, 
									    uint8_t oem_action,
									    uint8_t diagnostic_interrupt, 
									    fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_pef_configuration_parameters_pef_startup_delay (ipmi_ctx_t ctx, 
								    uint8_t pef_startup_delay, 
								    fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_pef_configuration_parameters_pef_alert_startup_delay (ipmi_ctx_t ctx, 
									  uint8_t pef_alert_startup_delay, 
									  fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_pef_configuration_parameters_event_filter_table (ipmi_ctx_t ctx, 
								     uint8_t filter_number,
								     uint8_t filter_configuration_type,
								     uint8_t filter_configuration_filter,
								     uint8_t event_filter_action_alert,
								     uint8_t event_filter_action_power_off,
								     uint8_t event_filter_action_reset,
								     uint8_t event_filter_action_power_cycle,
								     uint8_t event_filter_action_oem,
								     uint8_t event_filter_action_diagnostic_interrupt,
								     uint8_t event_filter_action_group_control_operation,
								     uint8_t alert_policy_number_policy_number,
								     uint8_t alert_policy_number_group_control_selector,
								     uint8_t event_severity,
								     uint8_t generator_id_byte1,
								     uint8_t generator_id_byte2,
								     uint8_t sensor_type,
								     uint8_t sensor_number,
								     uint8_t event_trigger,
								     uint16_t event_data1_offset_mask,
								     uint8_t event_data1_AND_mask,
								     uint8_t event_data1_compare1,
								     uint8_t event_data1_compare2,
								     uint8_t event_data2_AND_mask,
								     uint8_t event_data2_compare1,
								     uint8_t event_data2_compare2,
								     uint8_t event_data3_AND_mask,
								     uint8_t event_data3_compare1,
								     uint8_t event_data3_compare2,
								     fiid_obj_t obj_cmd_rs);
  
int8_t ipmi_cmd_set_pef_configuration_parameters_event_filter_table_data1_ (ipmi_ctx_t ctx, 
									    uint8_t filter_number,
									    uint8_t filter_configuration_type,
									    uint8_t filter_configuration_filter,
									    fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_pef_configuration_parameters_alert_policy_table (ipmi_ctx_t ctx, 
								      uint8_t alert_policy_entry_number, 
								      uint8_t policy_type, 
								      uint8_t policy_enabled, 
								      uint8_t policy_number, 
								      uint8_t destination_selector, 
								      uint8_t channel_number, 
								      uint8_t alert_string_set_selector, 
								      uint8_t event_specific_alert_string, 
								      fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_pef_configuration_parameters_alert_string_keys (ipmi_ctx_t ctx, 
								    uint8_t string_selector, 
								    uint8_t filter_number, 
								    uint8_t set_number_for_string, 
								    fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_pef_configuration_parameters_alert_strings (ipmi_ctx_t ctx, 
								uint8_t string_selector, 
								uint8_t block_selector, 
								uint8_t *string_data, 
								uint8_t string_data_len, 
								fiid_obj_t obj_cmd_rs);


int8_t ipmi_cmd_get_pef_configuration_parameters_pef_control (ipmi_ctx_t ctx, 
							      uint8_t get_parameter, 
							      uint8_t set_selector, 
							      uint8_t block_selector, 
							      fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_pef_configuration_parameters_pef_action_global_control (ipmi_ctx_t ctx, 
									    uint8_t get_parameter, 
									    uint8_t set_selector,
									    uint8_t block_selector, 
									    fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_pef_configuration_parameters_pef_startup_delay (ipmi_ctx_t ctx, 
								    uint8_t get_parameter, 
								    uint8_t set_selector,
								    uint8_t block_selector, 
								    fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_pef_configuration_parameters_pef_alert_startup_delay (ipmi_ctx_t ctx, 
									  uint8_t get_parameter, 
									  uint8_t set_selector,
									  uint8_t block_selector, 
									  fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_pef_configuration_parameters_number_of_event_filters (ipmi_ctx_t ctx, 
									  uint8_t get_parameter, 
									  uint8_t set_selector,
									  uint8_t block_selector, 
									  fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_pef_configuration_parameters_event_filter_table (ipmi_ctx_t ctx, 
								     uint8_t get_parameter, 
								     uint8_t set_selector,
								     uint8_t block_selector, 
								     fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_pef_configuration_parameters_event_filter_table_data1_ (ipmi_ctx_t ctx, 
									    uint8_t get_parameter, 
									    uint8_t set_selector, 
									    uint8_t block_selector, 
									    fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_pef_configuration_parameters_number_of_alert_policy_entries (ipmi_ctx_t ctx,
										 uint8_t get_parameter, 
										 uint8_t set_selector,
										 uint8_t block_selector, 
										 fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_pef_configuration_parameters_number_of_alert_strings (ipmi_ctx_t ctx, 
									  uint8_t get_parameter, 
									  uint8_t set_selector, 
									  uint8_t block_selector, 
									  fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_pef_configuration_parameters_alert_string_keys (ipmi_ctx_t ctx,
								    uint8_t get_parameter, 
								    uint8_t set_selector,
								    uint8_t block_selector, 
								    fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_pef_configuration_parameters_alert_string (ipmi_ctx_t ctx,
							       uint8_t get_parameter, 
							       uint8_t set_selector,
							       uint8_t block_selector, 
							       fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_pef_configuration_parameters_alert_policy_table (ipmi_ctx_t ctx, 
								     uint8_t get_parameter, 
								     uint8_t set_selector, 
								     uint8_t block_selector, 
								     fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_last_processed_event_id (ipmi_ctx_t ctx, 
					     uint8_t set_record_id_for_last_record,
					     uint16_t record_id,
					     fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_last_processed_event_id (ipmi_ctx_t ctx, 
					     fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_alert_immediate (ipmi_ctx_t ctx,
				 uint8_t channel_number,
				 uint8_t destination_selector,
				 uint8_t operation,
				 uint8_t string_selector,
				 uint8_t send_alert_string,
				 fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_pet_acknowledge (ipmi_ctx_t ctx, 
				 uint16_t sequence_number,
				 uint32_t local_timestamp,
				 uint8_t event_source_type,
				 uint8_t sensor_device,
				 uint8_t sensor_number,
				 uint32_t event_data,          
				 fiid_obj_t obj_cmd_rs);

#ifdef __cplusplus
}
#endif

#endif
