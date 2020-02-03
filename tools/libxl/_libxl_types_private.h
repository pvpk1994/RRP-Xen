#ifndef __LIBXL_TYPES_PRIVATE_H
#define __LIBXL_TYPES_PRIVATE_H

/*
 * DO NOT EDIT.
 *
 * This file is autogenerated by
 * "gentypes.py libxl_types.idl __libxl_types.h __libxl_types_private.h __libxl_types_json.h __libxl_types.c"
 */

int libxl__error_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_error *p);
int libxl__domain_type_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_domain_type *p);
int libxl__rdm_reserve_strategy_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_rdm_reserve_strategy *p);
int libxl__rdm_reserve_policy_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_rdm_reserve_policy *p);
int libxl__channel_connection_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_channel_connection *p);
int libxl__device_model_version_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_device_model_version *p);
int libxl__console_type_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_console_type *p);
int libxl__disk_format_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_disk_format *p);
int libxl__disk_backend_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_disk_backend *p);
int libxl__nic_type_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_nic_type *p);
int libxl__action_on_shutdown_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_action_on_shutdown *p);
int libxl__trigger_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_trigger *p);
int libxl__tsc_mode_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_tsc_mode *p);
int libxl__gfx_passthru_kind_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_gfx_passthru_kind *p);
int libxl__timer_mode_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_timer_mode *p);
int libxl__bios_type_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_bios_type *p);
int libxl__scheduler_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_scheduler *p);
int libxl__shutdown_reason_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_shutdown_reason *p);
int libxl__vga_interface_type_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vga_interface_type *p);
int libxl__vendor_device_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vendor_device *p);
int libxl__viridian_enlightenment_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_viridian_enlightenment *p);
int libxl__hdtype_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_hdtype *p);
int libxl__checkpointed_stream_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_checkpointed_stream *p);
int libxl__vuart_type_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vuart_type *p);
int libxl__vkb_backend_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vkb_backend *p);
int libxl__ioport_range_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_ioport_range *p);
int libxl__iomem_range_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_iomem_range *p);
int libxl__vga_interface_info_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vga_interface_info *p);
int libxl__vnc_info_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vnc_info *p);
int libxl__spice_info_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_spice_info *p);
int libxl__sdl_info_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_sdl_info *p);
int libxl__dominfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_dominfo *p);
int libxl__cpupoolinfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_cpupoolinfo *p);
int libxl__channelinfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_channelinfo *p);
int libxl__vminfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vminfo *p);
int libxl__version_info_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_version_info *p);
int libxl__domain_create_info_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_domain_create_info *p);
int libxl__domain_restore_params_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_domain_restore_params *p);
int libxl__sched_params_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_sched_params *p);
int libxl__vcpu_sched_params_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vcpu_sched_params *p);
int libxl__domain_sched_params_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_domain_sched_params *p);
int libxl__vnode_info_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vnode_info *p);
int libxl__gic_version_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_gic_version *p);
int libxl__rdm_reserve_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_rdm_reserve *p);
int libxl__altp2m_mode_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_altp2m_mode *p);
int libxl__domain_build_info_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_domain_build_info *p);
int libxl__device_vfb_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_device_vfb *p);
int libxl__device_vkb_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_device_vkb *p);
int libxl__device_disk_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_device_disk *p);
int libxl__device_nic_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_device_nic *p);
int libxl__device_pci_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_device_pci *p);
int libxl__device_rdm_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_device_rdm *p);
int libxl__usbctrl_type_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_usbctrl_type *p);
int libxl__usbdev_type_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_usbdev_type *p);
int libxl__device_usbctrl_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_device_usbctrl *p);
int libxl__device_usbdev_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_device_usbdev *p);
int libxl__device_dtdev_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_device_dtdev *p);
int libxl__device_vtpm_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_device_vtpm *p);
int libxl__device_p9_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_device_p9 *p);
int libxl__device_pvcallsif_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_device_pvcallsif *p);
int libxl__device_channel_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_device_channel *p);
int libxl__connector_param_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_connector_param *p);
int libxl__device_vdispl_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_device_vdispl *p);
int libxl__vsnd_pcm_format_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vsnd_pcm_format *p);
int libxl__vsnd_params_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vsnd_params *p);
int libxl__vsnd_stream_type_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vsnd_stream_type *p);
int libxl__vsnd_stream_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vsnd_stream *p);
int libxl__vsnd_pcm_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vsnd_pcm *p);
int libxl__device_vsnd_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_device_vsnd *p);
int libxl__domain_config_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_domain_config *p);
int libxl__diskinfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_diskinfo *p);
int libxl__nicinfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_nicinfo *p);
int libxl__vtpminfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vtpminfo *p);
int libxl__usbctrlinfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_usbctrlinfo *p);
int libxl__vcpuinfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vcpuinfo *p);
int libxl__physinfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_physinfo *p);
int libxl__connectorinfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_connectorinfo *p);
int libxl__vdisplinfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vdisplinfo *p);
int libxl__streaminfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_streaminfo *p);
int libxl__pcminfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_pcminfo *p);
int libxl__vsndinfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vsndinfo *p);
int libxl__vkbinfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_vkbinfo *p);
int libxl__numainfo_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_numainfo *p);
int libxl__cputopology_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_cputopology *p);
int libxl__pcitopology_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_pcitopology *p);
int libxl__sched_credit_params_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_sched_credit_params *p);
int libxl__sched_credit2_params_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_sched_credit2_params *p);
int libxl__domain_remus_info_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_domain_remus_info *p);
int libxl__event_type_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_event_type *p);
int libxl__event_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_event *p);
int libxl__psr_cmt_type_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_psr_cmt_type *p);
int libxl__psr_cbm_type_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_psr_cbm_type *p);
int libxl__psr_cat_info_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_psr_cat_info *p);
int libxl__psr_feat_type_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_psr_feat_type *p);
int libxl__psr_hw_info_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl_psr_hw_info *p);

#endif /* __LIBXL_TYPES_JSON_H */
