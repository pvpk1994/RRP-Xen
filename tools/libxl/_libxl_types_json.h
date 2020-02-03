#ifndef __LIBXL_TYPES_JSON_H
#define __LIBXL_TYPES_JSON_H

/*
 * DO NOT EDIT.
 *
 * This file is autogenerated by
 * "gentypes.py libxl_types.idl __libxl_types.h __libxl_types_private.h __libxl_types_json.h __libxl_types.c"
 */

yajl_gen_status libxl_error_gen_json(yajl_gen hand, libxl_error *p);
yajl_gen_status libxl_domain_type_gen_json(yajl_gen hand, libxl_domain_type *p);
yajl_gen_status libxl_rdm_reserve_strategy_gen_json(yajl_gen hand, libxl_rdm_reserve_strategy *p);
yajl_gen_status libxl_rdm_reserve_policy_gen_json(yajl_gen hand, libxl_rdm_reserve_policy *p);
yajl_gen_status libxl_channel_connection_gen_json(yajl_gen hand, libxl_channel_connection *p);
yajl_gen_status libxl_device_model_version_gen_json(yajl_gen hand, libxl_device_model_version *p);
yajl_gen_status libxl_console_type_gen_json(yajl_gen hand, libxl_console_type *p);
yajl_gen_status libxl_disk_format_gen_json(yajl_gen hand, libxl_disk_format *p);
yajl_gen_status libxl_disk_backend_gen_json(yajl_gen hand, libxl_disk_backend *p);
yajl_gen_status libxl_nic_type_gen_json(yajl_gen hand, libxl_nic_type *p);
yajl_gen_status libxl_action_on_shutdown_gen_json(yajl_gen hand, libxl_action_on_shutdown *p);
yajl_gen_status libxl_trigger_gen_json(yajl_gen hand, libxl_trigger *p);
yajl_gen_status libxl_tsc_mode_gen_json(yajl_gen hand, libxl_tsc_mode *p);
yajl_gen_status libxl_gfx_passthru_kind_gen_json(yajl_gen hand, libxl_gfx_passthru_kind *p);
yajl_gen_status libxl_timer_mode_gen_json(yajl_gen hand, libxl_timer_mode *p);
yajl_gen_status libxl_bios_type_gen_json(yajl_gen hand, libxl_bios_type *p);
yajl_gen_status libxl_scheduler_gen_json(yajl_gen hand, libxl_scheduler *p);
yajl_gen_status libxl_shutdown_reason_gen_json(yajl_gen hand, libxl_shutdown_reason *p);
yajl_gen_status libxl_vga_interface_type_gen_json(yajl_gen hand, libxl_vga_interface_type *p);
yajl_gen_status libxl_vendor_device_gen_json(yajl_gen hand, libxl_vendor_device *p);
yajl_gen_status libxl_viridian_enlightenment_gen_json(yajl_gen hand, libxl_viridian_enlightenment *p);
yajl_gen_status libxl_hdtype_gen_json(yajl_gen hand, libxl_hdtype *p);
yajl_gen_status libxl_checkpointed_stream_gen_json(yajl_gen hand, libxl_checkpointed_stream *p);
yajl_gen_status libxl_vuart_type_gen_json(yajl_gen hand, libxl_vuart_type *p);
yajl_gen_status libxl_vkb_backend_gen_json(yajl_gen hand, libxl_vkb_backend *p);
yajl_gen_status libxl_ioport_range_gen_json(yajl_gen hand, libxl_ioport_range *p);
yajl_gen_status libxl_iomem_range_gen_json(yajl_gen hand, libxl_iomem_range *p);
yajl_gen_status libxl_vga_interface_info_gen_json(yajl_gen hand, libxl_vga_interface_info *p);
yajl_gen_status libxl_vnc_info_gen_json(yajl_gen hand, libxl_vnc_info *p);
yajl_gen_status libxl_spice_info_gen_json(yajl_gen hand, libxl_spice_info *p);
yajl_gen_status libxl_sdl_info_gen_json(yajl_gen hand, libxl_sdl_info *p);
yajl_gen_status libxl_dominfo_gen_json(yajl_gen hand, libxl_dominfo *p);
yajl_gen_status libxl_cpupoolinfo_gen_json(yajl_gen hand, libxl_cpupoolinfo *p);
yajl_gen_status libxl_channelinfo_gen_json(yajl_gen hand, libxl_channelinfo *p);
yajl_gen_status libxl_vminfo_gen_json(yajl_gen hand, libxl_vminfo *p);
yajl_gen_status libxl_version_info_gen_json(yajl_gen hand, libxl_version_info *p);
yajl_gen_status libxl_domain_create_info_gen_json(yajl_gen hand, libxl_domain_create_info *p);
yajl_gen_status libxl_domain_restore_params_gen_json(yajl_gen hand, libxl_domain_restore_params *p);
yajl_gen_status libxl_sched_params_gen_json(yajl_gen hand, libxl_sched_params *p);
yajl_gen_status libxl_vcpu_sched_params_gen_json(yajl_gen hand, libxl_vcpu_sched_params *p);
yajl_gen_status libxl_domain_sched_params_gen_json(yajl_gen hand, libxl_domain_sched_params *p);
yajl_gen_status libxl_vnode_info_gen_json(yajl_gen hand, libxl_vnode_info *p);
yajl_gen_status libxl_gic_version_gen_json(yajl_gen hand, libxl_gic_version *p);
yajl_gen_status libxl_rdm_reserve_gen_json(yajl_gen hand, libxl_rdm_reserve *p);
yajl_gen_status libxl_altp2m_mode_gen_json(yajl_gen hand, libxl_altp2m_mode *p);
yajl_gen_status libxl_domain_build_info_gen_json(yajl_gen hand, libxl_domain_build_info *p);
yajl_gen_status libxl_device_vfb_gen_json(yajl_gen hand, libxl_device_vfb *p);
yajl_gen_status libxl_device_vkb_gen_json(yajl_gen hand, libxl_device_vkb *p);
yajl_gen_status libxl_device_disk_gen_json(yajl_gen hand, libxl_device_disk *p);
yajl_gen_status libxl_device_nic_gen_json(yajl_gen hand, libxl_device_nic *p);
yajl_gen_status libxl_device_pci_gen_json(yajl_gen hand, libxl_device_pci *p);
yajl_gen_status libxl_device_rdm_gen_json(yajl_gen hand, libxl_device_rdm *p);
yajl_gen_status libxl_usbctrl_type_gen_json(yajl_gen hand, libxl_usbctrl_type *p);
yajl_gen_status libxl_usbdev_type_gen_json(yajl_gen hand, libxl_usbdev_type *p);
yajl_gen_status libxl_device_usbctrl_gen_json(yajl_gen hand, libxl_device_usbctrl *p);
yajl_gen_status libxl_device_usbdev_gen_json(yajl_gen hand, libxl_device_usbdev *p);
yajl_gen_status libxl_device_dtdev_gen_json(yajl_gen hand, libxl_device_dtdev *p);
yajl_gen_status libxl_device_vtpm_gen_json(yajl_gen hand, libxl_device_vtpm *p);
yajl_gen_status libxl_device_p9_gen_json(yajl_gen hand, libxl_device_p9 *p);
yajl_gen_status libxl_device_pvcallsif_gen_json(yajl_gen hand, libxl_device_pvcallsif *p);
yajl_gen_status libxl_device_channel_gen_json(yajl_gen hand, libxl_device_channel *p);
yajl_gen_status libxl_connector_param_gen_json(yajl_gen hand, libxl_connector_param *p);
yajl_gen_status libxl_device_vdispl_gen_json(yajl_gen hand, libxl_device_vdispl *p);
yajl_gen_status libxl_vsnd_pcm_format_gen_json(yajl_gen hand, libxl_vsnd_pcm_format *p);
yajl_gen_status libxl_vsnd_params_gen_json(yajl_gen hand, libxl_vsnd_params *p);
yajl_gen_status libxl_vsnd_stream_type_gen_json(yajl_gen hand, libxl_vsnd_stream_type *p);
yajl_gen_status libxl_vsnd_stream_gen_json(yajl_gen hand, libxl_vsnd_stream *p);
yajl_gen_status libxl_vsnd_pcm_gen_json(yajl_gen hand, libxl_vsnd_pcm *p);
yajl_gen_status libxl_device_vsnd_gen_json(yajl_gen hand, libxl_device_vsnd *p);
yajl_gen_status libxl_domain_config_gen_json(yajl_gen hand, libxl_domain_config *p);
yajl_gen_status libxl_diskinfo_gen_json(yajl_gen hand, libxl_diskinfo *p);
yajl_gen_status libxl_nicinfo_gen_json(yajl_gen hand, libxl_nicinfo *p);
yajl_gen_status libxl_vtpminfo_gen_json(yajl_gen hand, libxl_vtpminfo *p);
yajl_gen_status libxl_usbctrlinfo_gen_json(yajl_gen hand, libxl_usbctrlinfo *p);
yajl_gen_status libxl_vcpuinfo_gen_json(yajl_gen hand, libxl_vcpuinfo *p);
yajl_gen_status libxl_physinfo_gen_json(yajl_gen hand, libxl_physinfo *p);
yajl_gen_status libxl_connectorinfo_gen_json(yajl_gen hand, libxl_connectorinfo *p);
yajl_gen_status libxl_vdisplinfo_gen_json(yajl_gen hand, libxl_vdisplinfo *p);
yajl_gen_status libxl_streaminfo_gen_json(yajl_gen hand, libxl_streaminfo *p);
yajl_gen_status libxl_pcminfo_gen_json(yajl_gen hand, libxl_pcminfo *p);
yajl_gen_status libxl_vsndinfo_gen_json(yajl_gen hand, libxl_vsndinfo *p);
yajl_gen_status libxl_vkbinfo_gen_json(yajl_gen hand, libxl_vkbinfo *p);
yajl_gen_status libxl_numainfo_gen_json(yajl_gen hand, libxl_numainfo *p);
yajl_gen_status libxl_cputopology_gen_json(yajl_gen hand, libxl_cputopology *p);
yajl_gen_status libxl_pcitopology_gen_json(yajl_gen hand, libxl_pcitopology *p);
yajl_gen_status libxl_sched_credit_params_gen_json(yajl_gen hand, libxl_sched_credit_params *p);
yajl_gen_status libxl_sched_credit2_params_gen_json(yajl_gen hand, libxl_sched_credit2_params *p);
yajl_gen_status libxl_domain_remus_info_gen_json(yajl_gen hand, libxl_domain_remus_info *p);
yajl_gen_status libxl_event_type_gen_json(yajl_gen hand, libxl_event_type *p);
yajl_gen_status libxl_event_gen_json(yajl_gen hand, libxl_event *p);
yajl_gen_status libxl_psr_cmt_type_gen_json(yajl_gen hand, libxl_psr_cmt_type *p);
yajl_gen_status libxl_psr_cbm_type_gen_json(yajl_gen hand, libxl_psr_cbm_type *p);
yajl_gen_status libxl_psr_cat_info_gen_json(yajl_gen hand, libxl_psr_cat_info *p);
yajl_gen_status libxl_psr_feat_type_gen_json(yajl_gen hand, libxl_psr_feat_type *p);
yajl_gen_status libxl_psr_hw_info_gen_json(yajl_gen hand, libxl_psr_hw_info *p);

#endif /* __LIBXL_TYPES_JSON_H */
