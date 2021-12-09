#ifndef __LIBXL_TYPES_INTERNAL_PRIVATE_H
#define __LIBXL_TYPES_INTERNAL_PRIVATE_H

/*
 * DO NOT EDIT.
 *
 * This file is autogenerated by
 * "gentypes.py libxl_types_internal.idl __libxl_types_internal.h __libxl_types_internal_private.h __libxl_types_internal_json.h __libxl_types_internal.c"
 */

_hidden int libxl___qmp_message_type_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl__qmp_message_type *p);
_hidden int libxl___device_kind_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl__device_kind *p);
_hidden int libxl___console_backend_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl__console_backend *p);
_hidden int libxl___device_console_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl__device_console *p);
_hidden int libxl___device_action_parse_json(libxl__gc *gc, const libxl__json_object *o, libxl__device_action *p);

#endif /* __LIBXL_TYPES_INTERNAL_JSON_H */