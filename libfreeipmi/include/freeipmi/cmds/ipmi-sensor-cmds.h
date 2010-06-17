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

#ifndef _IPMI_SENSOR_CMDS_H
#define _IPMI_SENSOR_CMDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <freeipmi/fiid/fiid.h>

#define IPMI_SENSOR_GET_SENSOR_COUNT  0x00
#define IPMI_SENSOR_GET_SDR_COUNT     0x01

#define IPMI_SENSOR_STATIC_SENSOR_POPULATION  0x0
#define IPMI_SENSOR_DYNAMIC_SENSOR_POPULATION 0x1

/* achu: as of IPMI 2.0 hysteresis_mask reserved for future - write as 0xFF */
#define IPMI_SENSOR_HYSTERESIS_MASK   0xFF

#define IPMI_SENSOR_EVENT_MESSAGE_ACTION_DO_NOT_CHANGE_INDIVIDUAL_ENABLES 0x0
#define IPMI_SENSOR_EVENT_MESSAGE_ACTION_ENABLE_SELECTED_EVENT_MESSAGES 0x1
#define IPMI_SENSOR_EVENT_MESSAGE_ACTION_DISABLE_SELECTED_EVENT_MESSAGES 0x2

#define IPMI_SENSOR_EVENT_MESSAGE_ACTION_VALID(__val) \
        (((__val) == IPMI_SENSOR_EVENT_MESSAGE_ACTION_DO_NOT_CHANGE_INDIVIDUAL_ENABLES \
          || (__val) == IPMI_SENSOR_EVENT_MESSAGE_ACTION_ENABLE_SELECTED_EVENT_MESSAGES \
          || (__val) == IPMI_SENSOR_EVENT_MESSAGE_ACTION_DISABLE_SELECTED_EVENT_MESSAGES) ? 1 : 0)

#define IPMI_SENSOR_SCANNING_ON_THIS_SENSOR_ENABLE   0x1
#define IPMI_SENSOR_SCANNING_ON_THIS_SENSOR_DISABLE  0x0

#define IPMI_SENSOR_SCANNING_ON_THIS_SENSOR_VALID(__val) \
        (((__val) == IPMI_SENSOR_SCANNING_ON_THIS_SENSOR_ENABLE \
          || (__val) == IPMI_SENSOR_SCANNING_ON_THIS_SENSOR_DISABLE) ? 1 : 0)

#define IPMI_SENSOR_ALL_EVENT_MESSAGES_ENABLE   0x1
#define IPMI_SENSOR_ALL_EVENT_MESSAGES_DISABLE  0x0

#define IPMI_SENSOR_ALL_EVENT_MESSAGES_VALID(__val) \
        (((__val) == IPMI_SENSOR_ALL_EVENT_MESSAGES_ENABLE \
          || (__val) == IPMI_SENSOR_ALL_EVENT_MESSAGES_DISABLE) ? 1 : 0)

#define IPMI_SENSOR_EVENT_FLAG_ENABLE  0x1
#define IPMI_SENSOR_EVENT_FLAG_DISABLE 0x0

#define IPMI_SENSOR_EVENT_FLAG_VALID(__val) \
        (((__val) == IPMI_SENSOR_EVENT_FLAG_ENABLE \
          || (__val) == IPMI_SENSOR_EVENT_FLAG_DISABLE) ? 1 : 0)

/* achu: Yes, this one is backwards.  I don't know why */
#define IPMI_SENSOR_RE_ARM_ALL_EVENT_STATUS_ENABLED 0x0
#define IPMI_SENSOR_RE_ARM_ALL_EVENT_STATUS_DISABLED 0x1

/* achu: Yes, this one is backwards.  I don't know why */
#define IPMI_SENSOR_READING_STATE_UNAVAILABLE 0x1
#define IPMI_SENSOR_READING_STATE_AVAILABLE 0x0

#define IPMI_SENSOR_THRESHOLD_SET     0x1
#define IPMI_SENSOR_THRESHOLD_NOT_SET 0x0

extern fiid_template_t tmpl_cmd_get_device_sdr_info_rq;
extern fiid_template_t tmpl_cmd_get_device_sdr_info_rs;
  
extern fiid_template_t tmpl_cmd_get_device_sdr_rq;
extern fiid_template_t tmpl_cmd_get_device_sdr_rs;

extern fiid_template_t tmpl_cmd_get_sensor_reading_factors_rq;
extern fiid_template_t tmpl_cmd_get_sensor_reading_factors_rs;

extern fiid_template_t tmpl_cmd_set_sensor_hysteresis_rq;
extern fiid_template_t tmpl_cmd_set_sensor_hysteresis_rs;

extern fiid_template_t tmpl_cmd_get_sensor_hysteresis_rq;
extern fiid_template_t tmpl_cmd_get_sensor_hysteresis_rs;

extern fiid_template_t tmpl_cmd_set_sensor_thresholds_rq;
extern fiid_template_t tmpl_cmd_set_sensor_thresholds_rs;

extern fiid_template_t tmpl_cmd_get_sensor_thresholds_rq;
extern fiid_template_t tmpl_cmd_get_sensor_thresholds_rs;

extern fiid_template_t tmpl_cmd_set_sensor_event_enable_rq;
extern fiid_template_t tmpl_cmd_set_sensor_event_enable_rs;
extern fiid_template_t tmpl_cmd_set_sensor_event_enable_threshold_rq;
extern fiid_template_t tmpl_cmd_set_sensor_event_enable_discrete_rq;

extern fiid_template_t tmpl_cmd_get_sensor_event_enable_rq;
extern fiid_template_t tmpl_cmd_get_sensor_event_enable_rs;
extern fiid_template_t tmpl_cmd_get_sensor_event_enable_threshold_rs;
extern fiid_template_t tmpl_cmd_get_sensor_event_enable_discrete_rs;

extern fiid_template_t tmpl_cmd_re_arm_sensor_events_rq;
extern fiid_template_t tmpl_cmd_re_arm_sensor_events_rs;
extern fiid_template_t tmpl_cmd_re_arm_sensor_events_threshold_rq;
extern fiid_template_t tmpl_cmd_re_arm_sensor_events_discrete_rq;

extern fiid_template_t tmpl_cmd_get_sensor_event_status_rq;
extern fiid_template_t tmpl_cmd_get_sensor_event_status_rs;
extern fiid_template_t tmpl_cmd_get_sensor_event_status_threshold_rq;
extern fiid_template_t tmpl_cmd_get_sensor_event_status_threshold_rs;
extern fiid_template_t tmpl_cmd_get_sensor_event_status_discerete_rq;
extern fiid_template_t tmpl_cmd_get_sensor_event_status_discerete_rs;

extern fiid_template_t tmpl_cmd_get_sensor_reading_rq;
extern fiid_template_t tmpl_cmd_get_sensor_reading_rs;
extern fiid_template_t tmpl_cmd_get_sensor_reading_threshold_rs;
extern fiid_template_t tmpl_cmd_get_sensor_reading_discrete_rs;

extern fiid_template_t tmpl_cmd_set_sensor_type_rq;
extern fiid_template_t tmpl_cmd_set_sensor_type_rs;

extern fiid_template_t tmpl_cmd_get_sensor_type_rq;
extern fiid_template_t tmpl_cmd_get_sensor_type_rs;

/* achu: as of IPMI 2.0 hysteresis_mask reserved for future - write as 0xFF */
int8_t fill_cmd_set_sensor_hysteresis (uint8_t sensor_number,
                                       uint8_t hysteresis_mask,
                                       uint8_t positive_going_threshold_hysteresis_value,
                                       uint8_t negative_going_threshold_hysteresis_value,
                                       fiid_obj_t obj_cmd_rq);

/* achu: as of IPMI 2.0 hysteresis_mask reserved for future - write as 0xFF */
int8_t fill_cmd_get_sensor_hysteresis (uint8_t sensor_number, 
                                       uint8_t hysteresis_mask,
                                       fiid_obj_t obj_cmd_rq);

int8_t fill_cmd_set_sensor_thresholds (uint8_t sensor_number,
                                       uint8_t *lower_non_critical_threshold,
                                       uint8_t *lower_critical_threshold,
                                       uint8_t *lower_non_recoverable_threshold,
                                       uint8_t *upper_non_critical_threshold,
                                       uint8_t *upper_critical_threshold,
                                       uint8_t *upper_non_recoverable_threshold,
                                       fiid_obj_t obj_cmd_rq);

int8_t fill_cmd_get_sensor_thresholds (uint8_t sensor_number, fiid_obj_t obj_cmd_rq);

int8_t fill_cmd_set_sensor_event_enable (uint8_t sensor_number,
                                         uint8_t event_message_action,
                                         uint8_t scanning_on_this_sensor,
                                         uint8_t all_event_messages,
                                         uint16_t assertion_event_bitmask,
                                         uint16_t deassertion_event_bitmask,
                                         fiid_obj_t obj_cmd_rq);

int8_t fill_cmd_set_sensor_event_enable_threshold (uint8_t sensor_number,
                                                   uint8_t event_message_action,
                                                   uint8_t scanning_on_this_sensor,
                                                   uint8_t all_event_messages,
                                                   uint8_t assertion_event_lower_non_critical_going_low, 
                                                   uint8_t assertion_event_lower_non_critical_going_high, 
                                                   uint8_t assertion_event_lower_critical_going_low, 
                                                   uint8_t assertion_event_lower_critical_going_high, 
                                                   uint8_t assertion_event_lower_non_recoverable_going_low, 
                                                   uint8_t assertion_event_lower_non_recoverable_going_high, 
                                                   uint8_t assertion_event_upper_non_critical_going_low, 
                                                   uint8_t assertion_event_upper_non_critical_going_high, 
                                                   uint8_t assertion_event_upper_critical_going_low, 
                                                   uint8_t assertion_event_upper_critical_going_high, 
                                                   uint8_t assertion_event_upper_non_recoverable_going_low, 
                                                   uint8_t assertion_event_upper_non_recoverable_going_high, 
                                                   uint8_t deassertion_event_lower_non_critical_going_low, 
                                                   uint8_t deassertion_event_lower_non_critical_going_high, 
                                                   uint8_t deassertion_event_lower_critical_going_low, 
                                                   uint8_t deassertion_event_lower_critical_going_high, 
                                                   uint8_t deassertion_event_lower_non_recoverable_going_low, 
                                                   uint8_t deassertion_event_lower_non_recoverable_going_high, 
                                                   uint8_t deassertion_event_upper_non_critical_going_low, 
                                                   uint8_t deassertion_event_upper_non_critical_going_high, 
                                                   uint8_t deassertion_event_upper_critical_going_low, 
                                                   uint8_t deassertion_event_upper_critical_going_high, 
                                                   uint8_t deassertion_event_upper_non_recoverable_going_low, 
                                                   uint8_t deassertion_event_upper_non_recoverable_going_high, 
                                                   fiid_obj_t obj_cmd_rq);

int8_t fill_cmd_set_sensor_event_enable_discrete (uint8_t sensor_number,
                                                  uint8_t event_message_action,
                                                  uint8_t scanning_on_this_sensor,
                                                  uint8_t all_event_messages,
                                                  uint8_t assertion_event_state_bit_0, 
                                                  uint8_t assertion_event_state_bit_1, 
                                                  uint8_t assertion_event_state_bit_2, 
                                                  uint8_t assertion_event_state_bit_3, 
                                                  uint8_t assertion_event_state_bit_4, 
                                                  uint8_t assertion_event_state_bit_5, 
                                                  uint8_t assertion_event_state_bit_6, 
                                                  uint8_t assertion_event_state_bit_7, 
                                                  uint8_t assertion_event_state_bit_8, 
                                                  uint8_t assertion_event_state_bit_9, 
                                                  uint8_t assertion_event_state_bit_10, 
                                                  uint8_t assertion_event_state_bit_11, 
                                                  uint8_t assertion_event_state_bit_12, 
                                                  uint8_t assertion_event_state_bit_13, 
                                                  uint8_t assertion_event_state_bit_14, 
                                                  uint8_t deassertion_event_state_bit_0, 
                                                  uint8_t deassertion_event_state_bit_1, 
                                                  uint8_t deassertion_event_state_bit_2, 
                                                  uint8_t deassertion_event_state_bit_3, 
                                                  uint8_t deassertion_event_state_bit_4, 
                                                  uint8_t deassertion_event_state_bit_5, 
                                                  uint8_t deassertion_event_state_bit_6, 
                                                  uint8_t deassertion_event_state_bit_7, 
                                                  uint8_t deassertion_event_state_bit_8, 
                                                  uint8_t deassertion_event_state_bit_9, 
                                                  uint8_t deassertion_event_state_bit_10, 
                                                  uint8_t deassertion_event_state_bit_11, 
                                                  uint8_t deassertion_event_state_bit_12, 
                                                  uint8_t deassertion_event_state_bit_13, 
                                                  uint8_t deassertion_event_state_bit_14, 
                                                  fiid_obj_t obj_cmd_rq);

int8_t fill_cmd_get_sensor_event_enable (uint8_t sensor_number, fiid_obj_t obj_cmd_rq);

int8_t fill_cmd_get_sensor_reading (uint8_t sensor_number, fiid_obj_t obj_cmd_rq);

#ifdef __cplusplus
}
#endif

#endif /* ipmi-sensor-cmds.h */