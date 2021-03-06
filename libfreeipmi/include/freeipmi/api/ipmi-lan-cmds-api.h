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

#ifndef _IPMI_LAN_CMDS_API_H
#define _IPMI_LAN_CMDS_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <freeipmi/api/ipmi-api.h>
#include <freeipmi/fiid/fiid.h>

int8_t ipmi_cmd_set_lan_configuration_parameters_authentication_type_enables (ipmi_ctx_t ctx, 
									      uint8_t channel_number, 
									      uint8_t callback_level_none,
									      uint8_t callback_level_md2,
									      uint8_t callback_level_md5,
									      uint8_t callback_level_straight_password,
									      uint8_t callback_level_oem_proprietary,
									      uint8_t user_level_none,
									      uint8_t user_level_md2,
									      uint8_t user_level_md5,
									      uint8_t user_level_straight_password,
									      uint8_t user_level_oem_proprietary,
									      uint8_t operator_level_none,
									      uint8_t operator_level_md2,
									      uint8_t operator_level_md5,
									      uint8_t operator_level_straight_password,
									      uint8_t operator_level_oem_proprietary,
									      uint8_t admin_level_none,
									      uint8_t admin_level_md2,
									      uint8_t admin_level_md5,
									      uint8_t admin_level_straight_password,
									      uint8_t admin_level_oem_proprietary,
									      uint8_t oem_level_none,
									      uint8_t oem_level_md2,
									      uint8_t oem_level_md5,
									      uint8_t oem_level_straight_password,
									      uint8_t oem_level_oem_proprietary,
									      fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_lan_configuration_parameters_ip_address (ipmi_ctx_t ctx, 
							     uint8_t channel_number, 
							     uint32_t ip_address, 
							     fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_lan_configuration_parameters_ip_address_source (ipmi_ctx_t ctx, 
								    uint8_t channel_number, 
								    uint8_t ip_address_source, 
								    fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_lan_configuration_parameters_subnet_mask (ipmi_ctx_t ctx, 
							      uint8_t channel_number, 
							      uint32_t subnet_mask, 
							      fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_lan_configuration_parameters_mac_address (ipmi_ctx_t ctx, 
							      uint8_t channel_number,
							      uint64_t mac_address,
							      fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_lan_configuration_parameters_default_gateway_address (ipmi_ctx_t ctx, 
									  uint8_t channel_number, 
									  uint32_t ip_address, 
									  fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_lan_configuration_parameters_default_gateway_mac_address (ipmi_ctx_t ctx, 
									      uint8_t channel_number,
									      uint64_t mac_address,
									      fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_lan_configuration_parameters_backup_gateway_address (ipmi_ctx_t ctx, 
									 uint8_t channel_number, 
									 uint32_t ip_address, 
									 fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_lan_configuration_parameters_backup_gateway_mac_address (ipmi_ctx_t ctx, 
									     uint8_t channel_number,
									     uint64_t mac_address,
									     fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_lan_configuration_parameters_bmc_generated_arp_control (ipmi_ctx_t ctx, 
									    uint8_t channel_number, 
									    uint8_t bmc_generated_gratuitous_arps, 
									    uint8_t bmc_generated_arp_responses, 
									    fiid_obj_t obj_cmd_rs);

int8_t ipmi_lan_set_lan_configuration_parameters_gratuitous_arp_interval (ipmi_ctx_t ctx, 
									  uint8_t channel_number, 
									  uint8_t gratuitous_arp_interval, 
									  fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_lan_configuration_parameters_community_string (ipmi_ctx_t ctx,
                                                                   uint8_t channel_number,
                                                                   char *community_string,
                                                                   unsigned int community_string_len,
                                                                   fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_lan_configuration_parameters_destination_type (ipmi_ctx_t ctx,
                                                                   uint8_t channel_number,
                                                                   uint8_t destination_selector,
                                                                   uint8_t destination_type,
                                                                   uint8_t alert_acknowledge,
                                                                   uint8_t alert_acknowledge_timeout,
                                                                   uint8_t retries,
                                                                   fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_lan_configuration_parameters_destination_addresses (ipmi_ctx_t ctx,
                                                                        uint8_t channel_number,
                                                                        uint8_t destination_selector,
                                                                        uint8_t gateway_selector,
                                                                        uint32_t alerting_ip_address,
                                                                        uint64_t alerting_mac_address,
                                                                        fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_lan_configuration_parameters_vlan_id (ipmi_ctx_t ctx, 
							  uint8_t channel_number, 
							  uint16_t vlan_id,
							  uint8_t vlan_id_enable, 
							  fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_lan_configuration_parameters_vlan_priority (ipmi_ctx_t ctx, 
								uint8_t channel_number,
								uint32_t vlan_priority,
								fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_set_lan_configuration_parameters_rmcpplus_messaging_cipher_suite_privilege_levels(ipmi_ctx_t ctx,
												  uint8_t channel_number,
												  uint8_t maximum_privilege_for_cipher_suite_1,
												  uint8_t maximum_privilege_for_cipher_suite_2,
												  uint8_t maximum_privilege_for_cipher_suite_3,
												  uint8_t maximum_privilege_for_cipher_suite_4,
												  uint8_t maximum_privilege_for_cipher_suite_5,
												  uint8_t maximum_privilege_for_cipher_suite_6,
												  uint8_t maximum_privilege_for_cipher_suite_7,
												  uint8_t maximum_privilege_for_cipher_suite_8,
												  uint8_t maximum_privilege_for_cipher_suite_9,
												  uint8_t maximum_privilege_for_cipher_suite_10,
												  uint8_t maximum_privilege_for_cipher_suite_11,
												  uint8_t maximum_privilege_for_cipher_suite_12,
												  uint8_t maximum_privilege_for_cipher_suite_13,
												  uint8_t maximum_privilege_for_cipher_suite_14,
												  uint8_t maximum_privilege_for_cipher_suite_15,
												  uint8_t maximum_privilege_for_cipher_suite_16,
												  fiid_obj_t obj_cmd_rs);


int8_t ipmi_cmd_get_lan_configuration_parameters_authentication_type_support (ipmi_ctx_t ctx, 
                                                                              uint8_t channel_number, 
                                                                              uint8_t get_parameter, 
                                                                              uint8_t set_selector, 
                                                                              uint8_t block_selector, 
                                                                              fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_authentication_type_enables (ipmi_ctx_t ctx, 
									      uint8_t channel_number, 
									      uint8_t get_parameter, 
									      uint8_t set_selector, 
									      uint8_t block_selector, 
									      fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_ip_address (ipmi_ctx_t ctx, 
							     uint8_t channel_number,
							     uint8_t get_parameter,
							     uint8_t set_selector,
							     uint8_t block_selector,
							     fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_ip_address_source (ipmi_ctx_t ctx, 
								    uint8_t channel_number, 
								    uint8_t get_parameter, 
								    uint8_t set_selector, 
								    uint8_t block_selector, 
								    fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_mac_address (ipmi_ctx_t ctx, 
							      uint8_t channel_number,
							      uint8_t get_parameter,
							      uint8_t set_selector,
							      uint8_t block_selector,
							      fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_subnet_mask (ipmi_ctx_t ctx, 
							      uint8_t channel_number,
							      uint8_t get_parameter,
							      uint8_t set_selector,
							      uint8_t block_selector,
							      fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_bmc_generated_arp_control (ipmi_ctx_t ctx, 
									    uint8_t channel_number, 
									    uint8_t get_parameter, 
									    uint8_t set_selector, 
									    uint8_t block_selector, 
									    fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_gratuitous_arp_interval (ipmi_ctx_t ctx, 
									  uint8_t channel_number, 
									  uint8_t get_parameter, 
									  uint8_t set_selector, 
									  uint8_t block_selector, 
									  fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_default_gateway_address (ipmi_ctx_t ctx, 
									  uint8_t channel_number,
									  uint8_t get_parameter,
									  uint8_t set_selector,
									  uint8_t block_selector,
									  fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_default_gateway_mac_address (ipmi_ctx_t ctx, 
									      uint8_t channel_number,
									      uint8_t get_parameter,
									      uint8_t set_selector,
									      uint8_t block_selector,
									      fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_backup_gateway_address (ipmi_ctx_t ctx, 
									 uint8_t channel_number,
									 uint8_t get_parameter,
									 uint8_t set_selector,
									 uint8_t block_selector,
									 fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_backup_gateway_mac_address (ipmi_ctx_t ctx, 
									     uint8_t channel_number,
									     uint8_t get_parameter,
									     uint8_t set_selector,
									     uint8_t block_selector,
									     fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_community_string (ipmi_ctx_t ctx,
                                                                   uint8_t channel_number,
                                                                   uint8_t get_parameter,
                                                                   uint8_t set_selector,
                                                                   uint8_t block_selector,
                                                                   fiid_obj_t obj_cmd_rs);


int8_t ipmi_cmd_get_lan_configuration_parameters_number_of_destinations (ipmi_ctx_t ctx,
                                                                         uint8_t channel_number,
                                                                         uint8_t get_parameter,
                                                                         uint8_t set_selector,
                                                                         uint8_t block_selector,
                                                                         fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_destination_type (ipmi_ctx_t ctx,
                                                                   uint8_t channel_number,
                                                                   uint8_t get_parameter,
                                                                   uint8_t set_selector,
                                                                   uint8_t block_selector,
                                                                   fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_destination_addresses (ipmi_ctx_t ctx,
                                                                        uint8_t channel_number,
                                                                        uint8_t get_parameter,
                                                                        uint8_t set_selector,
                                                                        uint8_t block_selector,
                                                                        fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_vlan_id (ipmi_ctx_t ctx, 
							  uint8_t channel_number, 
							  uint8_t get_parameter, 
							  uint8_t set_selector, 
							  uint8_t block_selector, 
							  fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_vlan_priority (ipmi_ctx_t ctx, 
								uint8_t channel_number, 
								uint8_t get_parameter, 
								uint8_t set_selector, 
								uint8_t block_selector, 
								fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_rmcpplus_messaging_cipher_suite_entry_support (ipmi_ctx_t ctx,
												uint8_t channel_number,
												uint8_t get_parameter,
												uint8_t set_selector,
												uint8_t block_selector,
												fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_rmcpplus_messaging_cipher_suite_entries (ipmi_ctx_t ctx,
											  uint8_t channel_number,
											  uint8_t get_parameter,
											  uint8_t set_selector,
											  uint8_t block_selector,
											  fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_lan_configuration_parameters_rmcpplus_messaging_cipher_suite_privilege_levels(ipmi_ctx_t ctx,
												  uint8_t channel_number,
												  uint8_t get_parameter,
												  uint8_t set_selector,
												  uint8_t block_selector,
												  fiid_obj_t obj_cmd_rs);


int8_t ipmi_cmd_suspend_bmc_arps (ipmi_ctx_t ctx, 
				  uint8_t channel_number, 
				  uint8_t gratuitous_arp_suspend, 
				  uint8_t arp_response_suspend, 
				  fiid_obj_t obj_cmd_rs);

int8_t ipmi_cmd_get_ip_udp_rmcp_statistics (ipmi_ctx_t ctx, 
                                            uint8_t channel_number, 
                                            uint8_t clear_all_statistics,
                                            fiid_obj_t obj_cmd_rs);

#ifdef __cplusplus
}
#endif

#endif
