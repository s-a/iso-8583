{
    "targets": [
        {
            "target_name": "ISO8583",
            "cflags_cc!": [ "-std=c++11" ],
            "sources": [ 
            	"NativeExtension.cc", "functions.cc", 
            	"lib/dl_iso8583.c",
				"lib/dl_iso8583_common.c",
				"lib/dl_iso8583_defs_1987.c",
				"lib/dl_iso8583_defs_1993.c",
				"lib/dl_iso8583_fields.c",
				"lib/dl_mem.c",
				"lib/dl_output.c",
				"lib/dl_str.c",
				"lib/dl_time.c",
				"lib/dl_timer.c"
            ],
            "include_dirs" : [
 	 			"<!(node -e \"require('nan')\")"
			]
        }
    ],
}