#ifndef OSI_DISABLE_GL_DISABLE
#ifndef OSI_GL_DISABLE
#define OSI_GL_DISABLE 1
/*
How to use this file:
1. Avoid #pragma once, use #ifndef OSI_GL_DISABLE (the old C way), because: if you use a custom file, #pragma once
   will include both the default osi file and your custom file. With the old C way this won't happen.
2. Each define will set a particular OpenGL extension #define to 0, signaling the oGL headers (glext.h/wglext.h/etc.h)
   not to declare that extension.
3. IT IS VERY IMPORTANT TO SET EACH DEFINE TO 0: this signals osi to not include anything about this extension. If it is 1,
   OpenGL header files will not define the extension, but osi will try to create functions and it won't work anymore

4. Basicaly just uncomment an extension/opengl version to disable it.
5. At the end of the file, if some base typedefs are missing that were created by an disabled extension and these
   typedefs are used in other extensions that are not disabled, they are handled there. (more testing should be done to check no dependancy is missing)

   This feature was suggested by reddit user /u/jringstad

TODO:
* incomplete extensions should be disabled, i think
* check out if anything can be done under mac, to disable stuff

*/


// >>> Simple rule: just uncomment something to disable it. <<<

// base OpenGL versions can be disabled, but i would avoid it

//#define GL_VERSION_1_1 0 /// disable OpenGL 1.1 funcs & everything
//#define GL_VERSION_1_2 0 /// disable OpenGL 1.2 funcs & everything
//#define GL_VERSION_1_3 0 /// disable OpenGL 1.3 funcs & everything
//#define GL_VERSION_1_4 0 /// disable OpenGL 1.4 funcs & everything
//#define GL_VERSION_1_5 0 /// disable OpenGL 1.5 funcs & everything
//#define GL_VERSION_2_0 0 /// disable OpenGL 2.0 funcs & everything
//#define GL_VERSION_2_1 0 /// disable OpenGL 2.1 funcs & everything
//#define GL_VERSION_3_0 0 /// disable OpenGL 3.0 funcs & everything
//#define GL_VERSION_3_1 0 /// disable OpenGL 3.1 funcs & everything
//#define GL_VERSION_3_2 0 /// disable OpenGL 3.2 funcs & everything
//#define GL_VERSION_3_3 0 /// disable OpenGL 3.3 funcs & everything
//#define GL_VERSION_4_0 0 /// disable OpenGL 4.0 funcs & everything
//#define GL_VERSION_4_1 0 /// disable OpenGL 4.1 funcs & everything
//#define GL_VERSION_4_2 0 /// disable OpenGL 4.2 funcs & everything
//#define GL_VERSION_4_3 0 /// disable OpenGL 4.3 funcs & everything
//#define GL_VERSION_4_4 0 /// disable OpenGL 4.4 funcs & everything


///=============================================================================///
// extensions not in ARB or EXT list - lost in space/time - hanging out in limbo //
///=============================================================================///

//#define GL_ARB_imaging 0
//#define GL_ARB_bindless_texture 0
//#define GL_INGR_blend_func_separate 0
//#define GL_SGIX_fragment_lighting 0
//#define GL_SGIX_polynomial_ffd 0
//#define GL_SGIS_point_parameters 0
//#define GL_SGIX_igloo_interface 0
//#define WGL_NV_vertex_array_range 0

//#define GL_ARB_multitexture             0 // 001 [] http://www.opengl.org/registry/specs/ARB/multitexture.txt
//#define GLX_ARB_get_proc_address        0 // 002 [returns the addr of GLX and GL ext funcs] http://www.opengl.org/registry/specs/ARB/get_proc_address.txt
//#define GL_ARB_transpose_matrix         0 // 003 [C-style matrices (transposed)] http://www.opengl.org/registry/specs/ARB/transpose_matrix.txt
//#define WGL_ARB_buffer_region           0 // 004 [allows an area of an oGL window to be saved in off-screen mem] http://www.opengl.org/registry/specs/ARB/wgl_buffer_region.txt
//#define GL_ARB_multisample              0 // 005 [Multisample antialiasing] http://www.opengl.org/registry/specs/ARB/multisample.txt
//#define GLX_ARB_multisample             0 // 005 [Multisample antialiasing] http://www.opengl.org/registry/specs/ARB/multisample.txt
//#define WGL_ARB_multisample             0 // 005 [Multisample antialiasing] http://www.opengl.org/registry/specs/ARB/multisample.txt
//#define GL_ARB_texture_env_add          0 // 006 [tex environment func ADD] http://www.opengl.org/registry/specs/ARB/texture_env_add.txt
//#define GL_ARB_texture_cube_map         0 // 007 [cube map textures] http://www.opengl.org/registry/specs/ARB/texture_cube_map.txt
//#define WGL_ARB_extensions_string       0 // 008 [apps can determine which WGL exts are supported] http://www.opengl.org/registry/specs/ARB/wgl_extensions_string.txt
//#define WGL_ARB_pixel_format            0 // 009 [pixel format funcs] http://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt
//#define WGL_ARB_make_current_read       0 // 010 [separate "read" and "draw" DC with the current context] http://www.opengl.org/registry/specs/ARB/wgl_make_current_read.txt
//#define WGL_ARB_pbuffer                 0 // 011 [defines pixel buffers] http://www.opengl.org/registry/specs/ARB/wgl_pbuffer.txt
//#define GL_ARB_texture_compression      0 // 012 [compressed textures framework (and some default compression modes)] http://www.opengl.org/registry/specs/ARB/texture_compression.txt
//#define GL_ARB_texture_border_clamp     0 // 013 [border clamp wrapping] http://www.opengl.org/registry/specs/ARB/texture_border_clamp.txt
//#define GL_ARB_point_parameters         0 // 014 [additional geometric characteristics of points] http://www.opengl.org/registry/specs/ARB/point_parameters.txt
//#define GL_ARB_vertex_blend             0 // 015 [vertex blend and weight funcs] http://www.opengl.org/registry/specs/ARB/vertex_blend.txt
//#define GL_ARB_matrix_palette           0 // 016 [extends vertex blend GL_ARB_vertex_blend] http://www.opengl.org/registry/specs/ARB/matrix_palette.txt
//#define GL_ARB_texture_env_combine      0 // 017 [new texture environment function] http://www.opengl.org/registry/specs/ARB/texture_env_combine.txt
//#define GL_ARB_texture_env_crossbar     0 // 018 [new texture environment function] http://www.opengl.org/registry/specs/ARB/texture_env_crossbar.txt
//#define GL_ARB_texture_env_dot3         0 // 019 [new operation to the texture combiner operations] http://www.opengl.org/registry/specs/ARB/texture_env_dot3.txt
//#define WGL_ARB_render_texture          0 // 020 [allows a color buffer to be used for both rendering and texturing] http://www.opengl.org/registry/specs/ARB/wgl_render_texture.txt
//#define GL_ARB_texture_mirrored_repeat  0 // 021 [tex wrap mode] http://www.opengl.org/registry/specs/ARB/texture_mirrored_repeat.txt
//#define GL_ARB_depth_texture            0 // 022 [depth texture format (ex: shadow casting)] http://www.opengl.org/registry/specs/ARB/depth_texture.txt
//#define GL_ARB_shadow                   0 // 023 [shadow maps] http://www.opengl.org/registry/specs/ARB/shadow.txt
//#define GL_ARB_shadow_ambient           0 // 024 [shadows, again!] http://www.opengl.org/registry/specs/ARB/shadow_ambient.txt
//#define GL_ARB_window_pos               0 // 025 [sets current raster pos in window coords] http://www.opengl.org/registry/specs/ARB/window_pos.txt
//#define GL_ARB_vertex_program           0 // 026 [vertex program!] http://www.opengl.org/registry/specs/ARB/vertex_program.txt
//#define GL_ARB_fragment_program         0 // 027 [fragment program!] http://www.opengl.org/registry/specs/ARB/fragment_program.txt
//#define GL_ARB_vertex_buffer_object     0 // 028 [VBO's !!!] http://www.opengl.org/registry/specs/ARB/vertex_buffer_object.txt
//#define GLX_ARB_vertex_buffer_object    0 // 028 [VBO's !!!] http://www.opengl.org/registry/specs/ARB/vertex_buffer_object.txt
//#define GL_ARB_occlusion_query          0 // 029 [determine the visibility of an object] http://www.opengl.org/registry/specs/ARB/occlusion_query.txt
//#define GL_ARB_shader_objects           0 // 030 [shader objects!] http://www.opengl.org/registry/specs/ARB/shader_objects.txt
//#define GL_ARB_vertex_shader            0 // 031 [vertex shader] http://www.opengl.org/registry/specs/ARB/vertex_shader.txt
//#define GL_ARB_fragment_shader          0 // 032 [fragment shader] http://www.opengl.org/registry/specs/ARB/fragment_shader.txt
//#define GL_ARB_shading_language_100     0 // 033 [shading language 1.00] http://www.opengl.org/registry/specs/ARB/shading_language_100.txt
//#define GL_ARB_texture_non_power_of_two 0 // 034 [non-power of two textures] http://www.opengl.org/registry/specs/ARB/texture_non_power_of_two.txt
//#define GL_ARB_point_sprite             0 // 035 [point sprites] http://www.opengl.org/registry/specs/ARB/point_sprite.txt
//#define GL_ARB_fragment_program_shadow  0 // 036 [fragment program shadow options] http://www.opengl.org/registry/specs/ARB/fragment_program_shadow.txt
//#define GL_ARB_draw_buffers             0 // 037 [extends ARB_fragment_program and ARB_fragment_shader] http://www.opengl.org/registry/specs/ARB/draw_buffers.txt
//#define GL_ARB_texture_rectangle        0 // 038 [another non-power of two type of texture] http://www.opengl.org/registry/specs/ARB/texture_rectangle.txt
//#define GL_ARB_color_buffer_float       0 // 039 [floating-point RGBA pixel format] http://www.opengl.org/registry/specs/ARB/color_buffer_float.txt
//#define WGL_ARB_pixel_format_float      0 // 039 [floating-point RGBA pixel format] http://www.opengl.org/registry/specs/ARB/color_buffer_float.txt
//#define GLX_ARB_fbconfig_float          0 // 039 [floating-point RGBA pixel format] http://www.opengl.org/registry/specs/ARB/color_buffer_float.txt
//#define GL_ARB_half_float_pixel         0 // 040 [half-precision floating-point for pixel data] http://www.opengl.org/registry/specs/ARB/half_float_pixel.txt
//#define GL_ARB_texture_float            0 // 041 [16&32bit floats for tex internal formats] http://www.opengl.org/registry/specs/ARB/texture_float.txt
//#define GL_ARB_pixel_buffer_object      0 // 042 [PBOs! accelerates work on pixels] http://www.opengl.org/registry/specs/ARB/pixel_buffer_object.txt
//#define GL_ARB_depth_buffer_float       0 // 043 [float32 for depth internal format] http://www.opengl.org/registry/specs/ARB/depth_buffer_float.txt
//#define GL_ARB_draw_instanced           0 // 044 [draw same object multiple times faster] http://www.opengl.org/registry/specs/ARB/draw_instanced.txt
//#define GL_ARB_framebuffer_object       0 // 045 3.0 [fastest / best extension for 'texture rendering'] http://www.opengl.org/registry/specs/ARB/framebuffer_object.txt
//#define GL_ARB_framebuffer_sRGB         0 // 046 [sRGB framebuffer update and blending] http://www.opengl.org/registry/specs/ARB/framebuffer_sRGB.txt
//#define GLX_ARB_framebuffer_sRGB        0 // 046 [sRGB framebuffer update and blending] http://www.opengl.org/registry/specs/ARB/framebuffer_sRGB.txt
//#define WGL_ARB_framebuffer_sRGB        0 // 046 [sRGB framebuffer update and blending] http://www.opengl.org/registry/specs/ARB/framebuffer_sRGB.txt
//#define GL_ARB_geometry_shader4         0 // 047 [a new shader type] http://www.opengl.org/registry/specs/ARB/geometry_shader4.txt
//#define GL_ARB_half_float_vertex        0 // 048 [float16 vertices] http://www.opengl.org/registry/specs/ARB/half_float_vertex.txt
//#define GL_ARB_instanced_arrays             0 // 049 [multiple draws of the same object] http://www.opengl.org/registry/specs/ARB/instanced_arrays.txt
//#define GL_ARB_map_buffer_range             0 // 050 3.0 [expands the buffer object API] http://www.opengl.org/registry/specs/ARB/map_buffer_range.txt
//#define GL_ARB_texture_buffer_object        0 // 051 [a new texture type] http://www.opengl.org/registry/specs/ARB/texture_buffer_object.txt
//#define GL_ARB_texture_compression_rgtc     0 // 052 [four new block-based texture compression formats] http://www.opengl.org/registry/specs/ARB/texture_compression_rgtc.txt
//#define GL_ARB_texture_rg                   0 // 053 [1&2 component texture internal format] http://www.opengl.org/registry/specs/ARB/texture_rg.txt
//#define GL_ARB_vertex_array_object          0 // 054 3.0 [named vertex array objects... hmm] http://www.opengl.org/registry/specs/ARB/vertex_array_object.txt
//#define WGL_ARB_create_context              0 // 055 BETTER NOT TO DISABLE THIS[!!! advanced context creation] http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
//#define WGL_ARB_create_context_profile      0 // 055 BETTER NOT TO DISABLE THIS[!!! advanced context creation] http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
//#define GLX_ARB_create_context              0 // 056 BETTER NOT TO DISABLE THIS[!!! advanced context creation] http://www.opengl.org/registry/specs/ARB/glx_create_context.txt
//#define GLX_ARB_create_context_profile      0 // 056 BETTER NOT TO DISABLE THIS[!!! advanced context creation] http://www.opengl.org/registry/specs/ARB/glx_create_context.txt
//#define GL_ARB_uniform_buffer_object        0 // 057 3.1 [GLSL uniforms known as a "uniform block"] http://www.opengl.org/registry/specs/ARB/uniform_buffer_object.txt
//#define GL_ARB_compatibility                0 // 058 !!! [this  enables pre 3.0 ogl deprecated stuff; ARB_create_context can set this extension off] http://www.opengl.org/registry/specs/ARB/compatibility.txt
//#define GL_ARB_copy_buffer                  0 // 059 3.1 [accelerated copy from one buffer object to another] http://www.opengl.org/registry/specs/ARB/copy_buffer.txt
//#define GL_ARB_shader_texture_lod           0 // 060 [adds texture functions to the GLSL] http://www.opengl.org/registry/specs/ARB/shader_texture_lod.txt
//#define GL_ARB_depth_clamp                  0 // 061 [near&far clipping stuff] http://www.opengl.org/registry/specs/ARB/depth_clamp.txt
//#define GL_ARB_draw_elements_base_vertex    0 // 062 3.2 [draw elements extended] http://www.opengl.org/registry/specs/ARB/draw_elements_base_vertex.txt
//#define GL_ARB_fragment_coord_conventions   0 // 063 [XY coords conventions] http://www.opengl.org/registry/specs/ARB/fragment_coord_conventions.txt
//#define GL_ARB_provoking_vertex             0 // 064 3.2 [first vertex has a special attr, check overview] http://www.opengl.org/registry/specs/ARB/provoking_vertex.txt
//#define GL_ARB_seamless_cube_map            0 // 065 [cube map stuff] http://www.opengl.org/registry/specs/ARB/seamless_cube_map.txt
//#define GL_ARB_sync                         0 // 066 3.2 [sync between the host CPU and the GPU] http://www.opengl.org/registry/specs/ARB/sync.txt
//#define GL_ARB_texture_multisample          0 // 067 3.2  [new types of "multisample textures"] http://www.opengl.org/registry/specs/ARB/texture_multisample.txt
//#define GL_ARB_vertex_array_bgra            0 // 068 [BGRA component ordering vertex attr] http://www.opengl.org/registry/specs/ARB/vertex_array_bgra.txt
//#define GL_ARB_draw_buffers_blend           0 // 069 [blend stuff for ARB_draw_buffers] http://www.opengl.org/registry/specs/ARB/draw_buffers_blend.txt
//#define GL_ARB_sample_shading               0 // 070 [GLSL stuff] http://www.opengl.org/registry/specs/ARB/sample_shading.txt
//#define GL_ARB_texture_cube_map_array       0 // 071 [array of cube map tex, i guess] http://www.opengl.org/registry/specs/ARB/texture_cube_map_array.txt
//#define GL_ARB_texture_gather               0 // 072 [GLSL tex funcs] http://www.opengl.org/registry/specs/ARB/texture_gather.txt
//#define GL_ARB_texture_query_lod            0 // 073 [fragment shader tex funcs] http://www.opengl.org/registry/specs/ARB/texture_query_lod.txt
//#define WGL_ARB_create_context              0 // 074 [context creation attribs] http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
//#define WGL_ARB_create_context_profile      0 // 074 [context creation attribs] http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
//#define GLX_ARB_create_context              0 // 075 [context creation attribs] http://www.opengl.org/registry/specs/ARB/glx_create_context.txt
//#define GL_ARB_shading_language_include     0 // 076 [#include GLSL directive] http://www.opengl.org/registry/specs/ARB/shading_language_include.txt
//#define GL_ARB_texture_compression_bptc     0 // 077 [BPTC and BPTC_FLOAT tex compression] http://www.opengl.org/registry/specs/ARB/texture_compression_bptc.txt
//#define GL_ARB_blend_func_extended          0 // 078 3.3 [new blending funcs] http://www.opengl.org/registry/specs/ARB/blend_func_extended.txt
//#define GL_ARB_explicit_attrib_location     0 // 079 ['pre-assign attrib locations to named vertex shader inputs..'] http://www.opengl.org/registry/specs/ARB/explicit_attrib_location.txt
//#define GL_ARB_occlusion_query2             0 // 080 [boolean occlusion] http://www.opengl.org/registry/specs/ARB/occlusion_query2.txt
//#define GL_ARB_sampler_objects              0 // 081 3.3 [(texture stuff, pretty good)A new object type is introduced, the sampler] http://www.opengl.org/registry/specs/ARB/sampler_objects.txt
//#define GL_ARB_shader_bit_encoding          0 // 082 [GLSL floating point funcs] http://www.opengl.org/registry/specs/ARB/shader_bit_encoding.txt
//#define GL_ARB_texture_rgb10_a2ui           0 // 083 [unsigned 10,10,10,10,2 int textures] http://www.opengl.org/registry/specs/ARB/texture_rgb10_a2ui.txt
//#define GL_ARB_texture_swizzle              0 // 084 [!!! tex swizzle] http://www.opengl.org/registry/specs/ARB/texture_swizzle.txt
//#define GL_ARB_timer_query                  0 // 085 3.3 [accurate timer] http://www.opengl.org/registry/specs/ARB/timer_query.txt
//#define GL_ARB_vertex_type_2_10_10_10_rev   0 // 086 3.3 [signed/unsigned 2.10.10.10 vert data format] http://www.opengl.org/registry/specs/ARB/vertex_type_2_10_10_10_rev.txt
//#define GL_ARB_draw_indirect                0 // 087 4.0 [supply args to DrawArraysInstanced / DrawElementsInstancedBaseVertex from buffer object memory] http://www.opengl.org/registry/specs/ARB/draw_indirect.txt
//#define GL_ARB_gpu_shader5                  0 // 088 [GLSL new feats] http://www.opengl.org/registry/specs/ARB/gpu_shader5.txt
//#define GL_ARB_gpu_shader_fp64              0 // 089 4.0 [GLSL shaders can use double (float64)] http://www.opengl.org/registry/specs/ARB/gpu_shader_fp64.txt
//#define GL_ARB_shader_subroutine            0 // 090 4.0 [shader stuff -"indirect subroutine calls"] http://www.opengl.org/registry/specs/ARB/shader_subroutine.txt
//#define GL_ARB_tessellation_shader          0 // 091 4.0 [tesselation shader stuff] http://www.opengl.org/registry/specs/ARB/tessellation_shader.txt
//#define GL_ARB_texture_buffer_object_rgb32  0 // 092 [new buffer texture formats] http://www.opengl.org/registry/specs/ARB/texture_buffer_object_rgb32.txt
//#define GL_ARB_transform_feedback2          0 // 093 4.0 [trans feedback extended] http://www.opengl.org/registry/specs/ARB/transform_feedback2.txt
//#define GL_ARB_transform_feedback3          0 // 094 4.0 [trans feedback extended] http://www.opengl.org/registry/specs/ARB/transform_feedback3.txt
//#define GL_ARB_ES2_compatibility            0 // 095 4.1 [adds features from OpenGL ES 2.0] http://www.opengl.org/registry/specs/ARB/ES2_compatibility.txt
//#define GL_ARB_get_program_binary           0 // 096 4.1 [GLSL program important stuff] http://www.opengl.org/registry/specs/ARB/get_program_binary.txt
//#define GL_ARB_separate_shader_objects      0 // 097 [GLSL stuff] http://www.opengl.org/registry/specs/ARB/separate_shader_objects.txt
//#define GL_ARB_shader_precision             0 // 098 [GLSL stuff] http://www.opengl.org/registry/specs/ARB/shader_precision.txt
//#define GL_ARB_vertex_attrib_64bit          0 // 099 4.1 [GLSL double (float64)] http://www.opengl.org/registry/specs/ARB/vertex_attrib_64bit.txt
//#define GL_ARB_viewport_array               0 // 100 4.1 [viewport/scissor stuff] http://www.opengl.org/registry/specs/ARB/viewport_array.txt
//#define GLX_ARB_create_context_robustness   0 // 101 ['robust' contexts & more options] http://www.opengl.org/registry/specs/ARB/glx_create_context_robustness.txt
//#define WGL_ARB_create_context_robustness   0 // 102 ['robust' contexts & more options] http://www.opengl.org/registry/specs/ARB/wgl_create_context_robustness.txt
//#define GL_ARB_cl_event                     0 // 103 [OpenGL sync objects linked to OpenCL] http://www.opengl.org/registry/specs/ARB/cl_event.txt
//#define GL_ARB_debug_output                 0 // 104 [oGL debbugging & notifier funcs] http://www.opengl.org/registry/specs/ARB/debug_output.txt
//#define GL_ARB_robustness                   0 // 105 ['robustness and security' for oGL contexts] http://www.opengl.org/registry/specs/ARB/robustness.txt
//#define GL_ARB_shader_stencil_export        0 // 106 [shader stencil stuff] http://www.opengl.org/registry/specs/ARB/shader_stencil_export.txt
//#define GL_ARB_base_instance                0 // 107 4.2 [DrawArrays more stuff] http://www.opengl.org/registry/specs/ARB/base_instance.txt
//#define GL_ARB_shading_language_420pack     0 // 108 [GLSL 4.20] http://www.opengl.org/registry/specs/ARB/shading_language_420pack.txt
//#define GL_ARB_transform_feedback_instanced 0 // 109 4.2 [DrawTransformFeedback stuff] http://www.opengl.org/registry/specs/ARB/transform_feedback_instanced.txt
//#define GL_ARB_compressed_texture_pixel_storage 0 // 110 [pixel store extension] http://www.opengl.org/registry/specs/ARB/compressed_texture_pixel_storage.txt
//#define GL_ARB_conservative_depth           0 // 111 [early depth test, can optimize stuff] http://www.opengl.org/registry/specs/ARB/conservative_depth.txt
//#define GL_ARB_internalformat_query         0 // 112 4.2 [more querry mechs (yes, the big robbots)] http://www.opengl.org/registry/specs/ARB/internalformat_query.txt
//#define GL_ARB_map_buffer_alignment         0 // 113 [map buffer works on 64b pointers] http://www.opengl.org/registry/specs/ARB/map_buffer_alignment.txt
//#define GL_ARB_shader_atomic_counters       0 // 114 4.2 [GLSL atomic counters] http://www.opengl.org/registry/specs/ARB/shader_atomic_counters.txt
//#define GL_ARB_shader_image_load_store      0 // 115 [GLSL texture stuff, memory sync stuff] http://www.opengl.org/registry/specs/ARB/shader_image_load_store.txt
//#define GL_ARB_shading_language_packing     0 // 116 [GLSL convert funcs] http://www.opengl.org/registry/specs/ARB/shading_language_packing.txt
//#define GL_ARB_texture_storage              0 // 117 [each tex level characteristics & mods, can set IMMUTABLE to a tex] http://www.opengl.org/registry/specs/ARB/texture_storage.txt
//#define GL_KHR_texture_compression_astc_hdr 0 // 118 [ASTC tex compression] http://www.opengl.org/registry/specs/KHR/texture_compression_astc_hdr.txt
//#define GL_KHR_texture_compression_astc_ldr 0 // 118 [ASTC tex compression] http://www.opengl.org/registry/specs/KHR/texture_compression_astc_hdr.txt
//#define GL_KHR_debug                        0 // 119 4.3 [oGL debug & notify funcs (seems to be like GL_ARB_debug_output)] http://www.opengl.org/registry/specs/KHR/debug.txt
//#define GL_ARB_arrays_of_arrays             0 // 120 [GLSL multi dimensional arrays (yeee!)] http://www.opengl.org/registry/specs/ARB/arrays_of_arrays.txt
//#define GL_ARB_clear_buffer_object          0 // 121 4.3 [adds a 'memset mechanism' to buffer objects] http://www.opengl.org/registry/specs/ARB/clear_buffer_object.txt
//#define GL_ARB_compute_shader               0 // 122 4.3 [! <funcs to use gr card for OTHER THAN DRAWING> !] http://www.opengl.org/registry/specs/ARB/compute_shader.txt
//#define GL_ARB_copy_image                   0 // 123 4.3 [a memcpy for the gr card (fastest possible, no pipeline messings etc)] http://www.opengl.org/registry/specs/ARB/copy_image.txt
//#define GL_ARB_texture_view                 0 // 124 4.3 [tex data can be 'view'ed in different ways] http://www.opengl.org/registry/specs/ARB/texture_view.txt
//#define GL_ARB_vertex_attrib_binding        0 // 125 4.3 [more vertex attr bindings] http://www.opengl.org/registry/specs/ARB/vertex_attrib_binding.txt
//#define GL_ARB_robustness_application_isolation 0 // 126 [context 'robustness' & more context attr] http://www.opengl.org/registry/specs/ARB/robustness_isolation.txt
//#define GL_ARB_robustness_share_group_isolation 0 // 126 [context 'robustness' & more context attr] http://www.opengl.org/registry/specs/ARB/robustness_isolation.txt
//#define GL_ARB_ES3_compatibility            0 // 127 [adds oGL ES 3.0 stuff to oGL3.x] http://www.opengl.org/registry/specs/ARB/ES3_compatibility.txt
//#define GL_ARB_explicit_uniform_location    0 // 128 [pre-assign uniform locations to unif vars] http://www.opengl.org/registry/specs/ARB/explicit_uniform_location.txt
//#define GL_ARB_fragment_layer_viewport      0 // 129 [adds gl_Layer and gl_ViewportIndex to frag shader] http://www.opengl.org/registry/specs/ARB/fragment_layer_viewport.txt
//#define GL_ARB_framebuffer_no_attachments   0 // 130 4.3 [fb extra stuff, configs etc(lots)] http://www.opengl.org/registry/specs/ARB/framebuffer_no_attachments.txt
//#define GL_ARB_internalformat_query2        0 // 131 [more querryes] http://www.opengl.org/registry/specs/ARB/internalformat_query2.txt
//#define GL_ARB_invalidate_subdata           0 // 132 4.3 [mark contents of a tex or buffer as no longer needed] http://www.opengl.org/registry/specs/ARB/invalidate_subdata.txt
//#define GL_ARB_multi_draw_indirect          0 // 133 4.3 [!! mass drawing of stuff] http://www.opengl.org/registry/specs/ARB/multi_draw_indirect.txt
//#define GL_ARB_program_interface_query      0 // 134 4.3 [! lots of more querryes] http://www.opengl.org/registry/specs/ARB/program_interface_query.txt
//#define GL_ARB_robust_buffer_access_behavior 0 // 135 [behaivour of these 'robust' contexts] http://www.opengl.org/registry/specs/ARB/robust_buffer_access_behavior.txt
//#define GL_ARB_shader_image_size            0 // 136 [GLSL funcs allowing shaders to query the size of an img] http://www.opengl.org/registry/specs/ARB/shader_image_size.txt
//#define GL_ARB_shader_storage_buffer_object 0 // 137 4.3 [shaders can do random access r/w and atomic mem ops on vars stored in a buffer object] http://www.opengl.org/registry/specs/ARB/shader_storage_buffer_object.txt
//#define GL_ARB_stencil_texturing            138 [resolves a depth/stencil problem] http://www.opengl.org/registry/specs/ARB/stencil_texturing.txt
//#define GL_ARB_texture_buffer_range         0 // 139 4.3 [extends ARB_texture_buffer_object] http://www.opengl.org/registry/specs/ARB/texture_buffer_range.txt
//#define GL_ARB_texture_query_levels         0 // 140 [GLSL texture querry funcs] http://www.opengl.org/registry/specs/ARB/texture_query_levels.txt
//#define GL_ARB_texture_storage_multisample  0 // 141 4.3 [immutable storage for multisampled texs] http://www.opengl.org/registry/specs/ARB/texture_storage_multisample.txt
//#define GLX_ARB_robustness_application_isolation 0 // 142 ['robust' contexts further config] http://www.opengl.org/registry/specs/ARB/glx_robustness_isolation.txt
//#define GLX_ARB_robustness_share_group_isolation 0 // 142 ['robust' contexts further config] http://www.opengl.org/registry/specs/ARB/glx_robustness_isolation.txt
//#define WGL_ARB_robustness_application_isolation 0 // 143 ['robust' contexts further config] http://www.opengl.org/registry/specs/ARB/wgl_robustness_isolation.txt
//#define WGL_ARB_robustness_share_group_isolation 0 // 143 ['robust' contexts further config] http://www.opengl.org/registry/specs/ARB/wgl_robustness_isolation.txt
//#define GL_ARB_buffer_storage               0 // 144 4.4 [immutable storage to buffer objects] http://www.opengl.org/registry/specs/ARB/buffer_storage.txt
//#define GL_ARB_clear_texture                0 // 145 4.4 [tex clearing (or set an initial val)] http://www.opengl.org/registry/specs/ARB/clear_texture.txt
//#define GL_ARB_enhanced_layouts             0 // 146 [has batman in overview!] http://www.opengl.org/registry/specs/ARB/enhanced_layouts.txt
//#define GL_ARB_multi_bind                   0 // 147 [bind multiple obj in a singe call] http://www.opengl.org/registry/specs/ARB/multi_bind.txt
//#define GL_ARB_query_buffer_object          0 // 148 [querrys return in buf mem, so shaders can access without a trip to normal mem] http://www.opengl.org/registry/specs/ARB/query_buffer_object.txt
//#define GL_ARB_texture_mirror_clamp_to_edge 0 // 149 [new texture mode, mirrors only once] http://www.opengl.org/registry/specs/ARB/texture_mirror_clamp_to_edge.txt
//#define GL_ARB_texture_stencil8             0 // 150 [STENCIL_INDEX8 tex internal format] http://www.opengl.org/registry/specs/ARB/texture_stencil8.txt
//#define GL_ARB_vertex_type_10f_11f_11f_rev  0 // 151 [new data format] http://www.opengl.org/registry/specs/ARB/vertex_type_10f_11f_11f_rev.txt
//#define GL_ARB_bindless_texture             0 // 152 4.4  [GLSL can access a tex without binding (faster)] http://www.opengl.org/registry/specs/ARB/bindless_texture.txt
//#define GL_ARB_compute_variable_group_size  0 // 153 [shader stuff] http://www.opengl.org/registry/specs/ARB/compute_variable_group_size.txt
//#define GL_ARB_indirect_parameters          0 // 154 [!extra stuff for mass drawing] http://www.opengl.org/registry/specs/ARB/indirect_parameters.txt
//#define GL_ARB_seamless_cubemap_per_texture 0 // 155 [cube map texture feature] http://www.opengl.org/registry/specs/ARB/seamless_cubemap_per_texture.txt
//#define GL_ARB_shader_draw_parameters       0 // 156 [shader stuff] http://www.opengl.org/registry/specs/ARB/shader_draw_parameters.txt
//#define GL_ARB_shader_group_vote            0 // 157 [shader stuff] http://www.opengl.org/registry/specs/ARB/shader_group_vote.txt
//#define GL_ARB_sparse_texture               0 // 158 [texture memory allocation management] http://www.opengl.org/registry/specs/ARB/sparse_texture.txt
//#define GL_ARB_ES3_1_compatibility          0 // 159 [OpenGL ES stuff, for easier porting] http://www.opengl.org/registry/specs/ARB/ES3_1_compatibility.txt
//#define GL_ARB_clip_control                 0 // 160 [Direct3D's coordinate system & improve precision of the Z coord mapping] http://www.opengl.org/registry/specs/ARB/clip_control.txt
//#define GL_ARB_conditional_render_inverted  0 // 161 [new modes to BeginConditionalRender] http://www.opengl.org/registry/specs/ARB/conditional_render_inverted.txt
//#define GL_ARB_cull_distance                0 // 162 [new GLSL gl_CullDistance shader output] http://www.opengl.org/registry/specs/ARB/cull_distance.txt
//#define GL_ARB_derivative_control           0 // 163 ['This extension provides control over the spacial granularity at which the underlying implementation computes derivatives.' ... shoot me!] http://www.opengl.org/registry/specs/ARB/derivative_control.txt
//#define GL_ARB_direct_state_access          0 // 164 [no need to bind an object to a context to modify it(big ext)] http://www.opengl.org/registry/specs/ARB/direct_state_access.txt
//#define GL_ARB_get_texture_sub_image        0 // 165 [funcs to get sub-regions of textures] http://www.opengl.org/registry/specs/ARB/get_texture_sub_image.txt
//#define GL_ARB_shader_texture_image_samples 0 // 166 [GLSL funcs allowing shaders to query the nr of samples of a tex] http://www.opengl.org/registry/specs/ARB/shader_texture_image_samples.txt
//#define GL_ARB_texture_barrier              0 // 167 [relaxes restrictions on rendering to a bound tex & mechanism to avoid read-after-write hazards] http://www.opengl.org/registry/specs/ARB/texture_barrier.txt
//#define GL_KHR_context_flush_control        0 // 168 [!!! new context creation parameters (no more flushes after a context switch for example)] http://www.opengl.org/registry/specs/KHR/context_flush_control.txt
//#define GLX_ARB_context_flush_control       0 // 168 [!!! new context creation parameters (no more flushes after a context switch for example)] http://www.opengl.org/registry/specs/KHR/context_flush_control.txt
//#define WGL_ARB_context_flush_control       0 // 168 [!!! new context creation parameters (no more flushes after a context switch for example)] http://www.opengl.org/registry/specs/KHR/context_flush_control.txt
//#define GL_KHR_robust_buffer_access_behavior 0 // 169 [no more crashes when an access of out of bonds buffer and array access] http://www.opengl.org/registry/specs/KHR/robust_buffer_access_behavior.txt
//#define GL_KHR_robustness                   0 // 170 [fast check of the context status (crashed, reset etc)/ safe checks, if the gr card accessed out of bounds stuff] http://www.opengl.org/registry/specs/KHR/robustness.txt
//#define GL_ARB_pipeline_statistics_query    0 // 171 [new query types that allow apps to get statistics information] http://www.opengl.org/registry/specs/ARB/pipeline_statistics_query.txt
//#define GL_ARB_sparse_buffer                0 // 172 ['create partially populated textures that would over-subscribe available graphics memory if made fully resident' ... or something, as Butthead would say] http://www.opengl.org/registry/specs/ARB/sparse_buffer.txt
//#define GL_ARB_transform_feedback_overflow_query 0 // 173 [new queries to detect overflows of transform feedback buffers] http://www.opengl.org/registry/specs/ARB/transform_feedback_overflow_query.txt















///=============================================================================///
// EXT extension list =========================--------------------------------- //
///=============================================================================///


//#define GL_EXT_abgr                        0 // 001 [] http://www.opengl.org/registry/specs/EXT/abgr.txt
//#define GL_EXT_blend_color                 0 // 002 [] http://www.opengl.org/registry/specs/EXT/blend_color.txt
//#define GL_EXT_polygon_offset              0 // 003 [] http://www.opengl.org/registry/specs/EXT/polygon_offset.txt
//#define GL_EXT_texture                     0 // 004 [] http://www.opengl.org/registry/specs/EXT/texture.txt
//#define GL_EXT_texture3D                   0 // 006 [] http://www.opengl.org/registry/specs/EXT/texture3D.txt
//#define GL_SGIS_texture_filter4            0 // 007 [] http://www.opengl.org/registry/specs/SGIS/texture_filter4.txt
//#define GL_EXT_subtexture                  0 // 009 [] http://www.opengl.org/registry/specs/EXT/subtexture.txt
//#define GL_EXT_copy_texture                0 // 010 [] http://www.opengl.org/registry/specs/EXT/copy_texture.txt
//#define GL_EXT_histogram                   0 // 011 [] http://www.opengl.org/registry/specs/EXT/histogram.txt
//#define GL_EXT_convolution                 0 // 012 [] http://www.opengl.org/registry/specs/EXT/convolution.txt
//#define GL_SGI_color_matrix                0 // 013 [] http://www.opengl.org/registry/specs/SGI/color_matrix.txt
//#define GL_SGI_color_table                 0 // 014 [] http://www.opengl.org/registry/specs/SGI/color_table.txt
//#define GL_SGIS_pixel_texture              0 // 015 [] http://www.opengl.org/registry/specs/SGIS/pixel_texture.txt http://www.opengl.org/registry/specs/SGIX/sgix_pixel_texture.txt
//#define GL_SGIX_pixel_texture              0 // ^015a http://www.opengl.org/registry/specs/SGIS/pixel_texture.txt http://www.opengl.org/registry/specs/SGIX/sgix_pixel_texture.txt
//#define GL_SGIS_texture4D                  0 // 016 [] http://www.opengl.org/registry/specs/SGIS/texture4D.txt
//#define GL_SGI_texture_color_table         0 // 017 [] http://www.opengl.org/registry/specs/SGI/texture_color_table.txt
//#define GL_EXT_cmyka                       0 // 018 [] http://www.opengl.org/registry/specs/EXT/cmyka.txt
//#define GL_EXT_texture_object              0 // 020 [] http://www.opengl.org/registry/specs/EXT/texture_object.txt
//#define GL_SGIS_detail_texture             0 // 021 [] http://www.opengl.org/registry/specs/SGIS/detail_texture.txt
//#define GL_SGIS_sharpen_texture            0 // 022 [] http://www.opengl.org/registry/specs/SGIS/sharpen_texture.txt
//#define GL_EXT_packed_pixels               0 // 023 [] http://www.opengl.org/registry/specs/EXT/packed_pixels.txt
//#define GL_SGIS_texture_lod                0 // 024 [] http://www.opengl.org/registry/specs/SGIS/texture_lod.txt
//#define GL_SGIS_multisample                0 // 025 [] http://www.opengl.org/registry/specs/SGIS/multisample.txt
//#define GLX_SGIS_multisample               0 // ^025
//#define GL_EXT_rescale_normal              0 // 027 [] http://www.opengl.org/registry/specs/EXT/rescale_normal.txt
//#define GLX_EXT_visual_info                0 // 028 [] http://www.opengl.org/registry/specs/EXT/visual_info.txt
//#define GL_EXT_vertex_array                0 // 030 [] http://www.opengl.org/registry/specs/EXT/vertex_array.txt
//#define GL_EXT_misc_attribute              0 // 031 [] http://www.opengl.org/registry/specs/EXT/misc_attribute.txt
//#define GL_SGIS_generate_mipmap            0 // 032 [] http://www.opengl.org/registry/specs/SGIS/generate_mipmap.txt
#define GL_SGIX_clipmap                    0 // 033 [incomplete] http://www.opengl.org/registry/specs/SGIX/clipmap.txt
#define GL_SGIX_shadow                     0 // 034 [incomplete] http://www.opengl.org/registry/specs/SGIX/shadow.txt
//#define GL_SGIS_texture_edge_clamp         0 // 035 [] http://www.opengl.org/registry/specs/SGIS/texture_edge_clamp.txt
//#define GL_SGIS_texture_border_clamp       0 // 036 [] http://www.opengl.org/registry/specs/SGIS/texture_border_clamp.txt
//#define GL_EXT_blend_minmax                0 // 037 [] http://www.opengl.org/registry/specs/EXT/blend_minmax.txt
//#define GL_EXT_blend_subtract              0 // 038 [] http://www.opengl.org/registry/specs/EXT/blend_subtract.txt
//#define GL_EXT_blend_logic_op              0 // 039 [] http://www.opengl.org/registry/specs/EXT/blend_logic_op.txt
//#define GLX_SGI_swap_control               0 // 040 [] http://www.opengl.org/registry/specs/SGI/swap_control.txt
//#define GLX_SGI_video_sync                 0 // 041 [] http://www.opengl.org/registry/specs/SGI/video_sync.txt
//#define GLX_SGI_make_current_read          0 // 042 [] http://www.opengl.org/registry/specs/SGI/make_current_read.txt
//#define GLX_SGIX_video_source              0 // 043 [] http://www.opengl.org/registry/specs/SGIX/video_source.txt
//#define GLX_EXT_visual_rating              0 // 044 [] http://www.opengl.org/registry/specs/EXT/visual_rating.txt
//#define GL_SGIX_interlace                  0 // 045 [] http://www.opengl.org/registry/specs/SGIX/interlace.txt
//#define GLX_EXT_import_context             0 // 047 [] http://www.opengl.org/registry/specs/EXT/import_context.txt
//#define GLX_SGIX_fbconfig                  0 // 049 [] http://www.opengl.org/registry/specs/SGIX/fbconfig.txt
//#define GLX_SGIX_pbuffer                   0 // 050 [] http://www.opengl.org/registry/specs/SGIX/pbuffer.txt
//#define GL_SGIS_texture_select             0 // 051 [] http://www.opengl.org/registry/specs/SGIS/texture_select.txt
//#define GL_SGIX_sprite                     0 // 052 [] http://www.opengl.org/registry/specs/SGIX/sprite.txt
//#define GL_SGIX_texture_multi_buffer       0 // 053 [] http://www.opengl.org/registry/specs/SGIX/texture_multi_buffer.txt
//#define GL_EXT_point_parameters            0 // 054 [] http://www.opengl.org/registry/specs/EXT/point_parameters.txt
//#define GL_SGIX_instruments                0 // 055 [] http://www.opengl.org/registry/specs/SGIX/instruments.txt
//#define GL_SGIX_texture_scale_bias         0 // 056 [] http://www.opengl.org/registry/specs/SGIX/texture_scale_bias.txt
//#define GL_SGIX_framezoom                  0 // 057 [] http://www.opengl.org/registry/specs/SGIX/framezoom.txt
//#define GL_SGIX_tag_sample_buffer          0 // 058 [] http://www.opengl.org/registry/specs/SGIX/tag_sample_buffer.txt
//#define GL_SGIX_reference_plane            0 // 060 [] http://www.opengl.org/registry/specs/SGIX/reference_plane.txt
//#define GL_SGIX_flush_raster               0 // 061 [] http://www.opengl.org/registry/specs/SGIX/flush_raster.txt
//#define GLX_SGI_cushion                    0 // 062 [] http://www.opengl.org/registry/specs/SGI/cushion.txt
//#define GL_SGIX_depth_texture              0 // 063 [] http://www.opengl.org/registry/specs/SGIX/depth_texture.txt
//#define GL_SGIS_fog_function               0 // 064 [] http://www.opengl.org/registry/specs/SGIS/fog_func.txt
//#define GL_SGIX_fog_offset                 0 // 065 [] http://www.opengl.org/registry/specs/SGIX/fog_offset.txt
//#define GL_HP_image_transform              0 // 066 [] http://www.opengl.org/registry/specs/HP/image_transform.txt
//#define GL_HP_convolution_border_modes     0 // 067 [] http://www.opengl.org/registry/specs/HP/convolution_border_modes.txt
//#define GL_SGIX_texture_add_env            0 // 069 [] http://www.opengl.org/registry/specs/SGIX/texture_env_add.txt
//#define GL_EXT_color_subtable              0 // 074 [] http://www.opengl.org/registry/specs/EXT/color_subtable.txt
//#define GLU_EXT_object_space_tess          0 // 075 [] http://www.opengl.org/registry/specs/EXT/object_space_tess.txt
//#define GL_PGI_vertex_hints                0 // 076 [] http://www.opengl.org/registry/specs/PGI/vertex_hints.txt
//#define GL_PGI_misc_hints                  0 // 077 [] http://www.opengl.org/registry/specs/PGI/misc_hints.txt
//#define GL_EXT_paletted_texture            0 // 078 [] http://www.opengl.org/registry/specs/EXT/paletted_texture.txt
//#define GL_EXT_clip_volume_hint            0 // 079 [] http://www.opengl.org/registry/specs/EXT/clip_volume_hint.txt
#define GL_SGIX_list_priority              0 // 080 [incomplete] http://www.opengl.org/registry/specs/SGIX/list_priority.txt
//#define GL_SGIX_ir_instrument1             0 // 081 [] http://www.opengl.org/registry/specs/SGIX/ir_instrument1.txt
//#define GLX_SGIX_video_resize              0 // 083 [] http://www.opengl.org/registry/specs/SGIX/video_resize.txt
//#define GL_SGIX_texture_lod_bias           0 // 084 [] http://www.opengl.org/registry/specs/SGIX/texture_lod_bias.txt
#define GLU_SGI_filter4_parameters         0 // 085 [incomplete] http://www.opengl.org/registry/specs/SGI/filter4_parameters.txt
#define GLX_SGIX_dm_buffer                 0 // 086 [incomplete] http://www.opengl.org/registry/specs/SGIX/dmbuffer.txt
//#define GLX_SGIX_dmbuffer                  0 // ^086
//#define GL_SGIX_shadow_ambient             0 // 090 [] http://www.opengl.org/registry/specs/SGIX/shadow_ambient.txt
//#define GLX_SGIX_swap_group                0 // 091 [] http://www.opengl.org/registry/specs/SGIX/swap_group.txt
//#define GLX_SGIX_swap_barrier              0 // 092 [] http://www.opengl.org/registry/specs/SGIX/swap_barrier.txt
//#define GL_EXT_index_texture               0 // 093 [] http://www.opengl.org/registry/specs/EXT/index_texture.txt
//#define GL_EXT_index_material              0 // 094 [] http://www.opengl.org/registry/specs/EXT/index_material.txt
//#define GL_EXT_index_func                  0 // 095 [] http://www.opengl.org/registry/specs/EXT/index_func.txt
#define GL_EXT_index_array_formats         0 // 096 [incomplete] http://www.opengl.org/registry/specs/EXT/index_array_formats.txt
#define GL_EXT_compiled_vertex_array       0 // 097 [incomplete] http://www.opengl.org/registry/specs/EXT/compiled_vertex_array.txt
#define GL_EXT_cull_vertex                 0 // 098 [incomplete] http://www.opengl.org/registry/specs/EXT/cull_vertex.txt
//#define GLU_EXT_nurbs_tessellator          0 // 100 [] #100 http://www.opengl.org/registry/specs/EXT/nurbs_tessellator.txt
#define GL_SGIX_ycrcb                      0 // 101 [incomplete] http://www.opengl.org/registry/specs/SGIX/ycrcb.txt
#define GL_EXT_fragment_lighting           0 // 102 [incomplete] http://www.opengl.org/registry/specs/EXT/fragment_lighting.txt
//#define GL_IBM_rasterpos_clip              0 // 110 [] http://www.opengl.org/registry/specs/IBM/rasterpos_clip.txt
#define GL_HP_texture_lighting             0 // 111 [incomplete] http://www.opengl.org/registry/specs/HP/texture_lighting.txt
//#define GL_EXT_draw_range_elements         0 // 112 [] http://www.opengl.org/registry/specs/EXT/draw_range_elements.txt
//#define GL_WIN_phong_shading               0 // 113 [] http://www.opengl.org/registry/specs/WIN/phong_shading.txt
//#define GL_WIN_specular_fog                0 // 114 [] http://www.opengl.org/registry/specs/WIN/specular_fog.txt
//#define GL_SGIS_color_range                0 // 115 [] http://www.opengl.org/registry/specs/SGIS/color_range.txt
//#define GLX_SGIS_color_range               0 // ^115
//#define GL_EXT_light_texture               0 // 117 [] http://www.opengl.org/registry/specs/EXT/light_texture.txt
#define GL_SGIX_blend_alpha_minmax         0 // 119 [incomplete] http://www.opengl.org/registry/specs/SGIX/blend_alpha_minmax.txt
//#define GL_EXT_scene_marker                0 // 120 [] http://www.opengl.org/registry/specs/EXT/scene_marker.txt
//#define GLX_EXT_scene_marker               0 // ^120
//#define GL_SGIX_pixel_texture_bits         0 // 127 [] http://www.opengl.org/registry/specs/SGIX/pixel_texture_bits.txt
//#define GL_EXT_bgra                        0 // 129 [] http://www.opengl.org/registry/specs/EXT/bgra.txt
#define GL_SGIX_async                      0 // 132 [incomplete] http://www.opengl.org/registry/specs/SGIX/async.txt
#define GL_SGIX_async_pixel                0 // 133 [inc] http://www.opengl.org/registry/specs/SGIX/async_pixel.txt
#define GL_SGIX_async_histogram            0 // 134 [inc] http://www.opengl.org/registry/specs/SGIX/async_histogram.txt
//#define GL_INTEL_texture_scissor           0 // 135 [] http://www.opengl.org/registry/specs/INTEL/texture_scissor.txt
//#define GL_INTEL_parallel_arrays           0 // 136 [] http://www.opengl.org/registry/specs/INTEL/parallel_arrays.txt
#define GL_HP_occlusion_test               0 // 137 inc[] http://www.opengl.org/registry/specs/HP/occlusion_test.txt
//#define GL_EXT_pixel_transform             0 // 138 [] http://www.opengl.org/registry/specs/EXT/pixel_transform.txt
//#define GL_EXT_pixel_transform_color_table 0 // 139 [] http://www.opengl.org/registry/specs/EXT/pixel_transform_color_table.txt
//#define GL_EXT_shared_texture_palette      0 // 141 [] http://www.opengl.org/registry/specs/EXT/shared_texture_palette.txt
//#define GLX_SGIS_blended_overlay           0 // 142 [] http://www.opengl.org/registry/specs/SGIS/blended_overlay.txt
//#define GL_EXT_separate_specular_color     0 // 144 [] http://www.opengl.org/registry/specs/EXT/separate_specular_color.txt
//#define GL_EXT_secondary_color             0 // 145 [] http://www.opengl.org/registry/specs/EXT/secondary_color.txt
//#define GL_EXT_texture_env                 0 // 146 [] http://www.opengl.org/registry/specs/EXT/texture_env.txt
//#define GL_EXT_texture_perturb_normal      0 // 147 [] http://www.opengl.org/registry/specs/EXT/texture_perturb_normal.txt
//#define GL_EXT_multi_draw_arrays           0 // 148 [] http://www.opengl.org/registry/specs/EXT/multi_draw_arrays.txt
//#define GL_SUN_multi_draw_arrays           0 // ^148
//#define GL_EXT_fog_coord                   0 // 149 [] http://www.opengl.org/registry/specs/EXT/fog_coord.txt
#define GL_REND_screen_coordinates         0 // 155 [inc] http://www.opengl.org/registry/specs/REND/screen_coordinates.txt
#define GL_EXT_coordinate_frame            0 // 156 [inc] http://www.opengl.org/registry/specs/EXT/coordinate_frame.txt
//#define GL_EXT_texture_env_combine         0 // 158 [] http://www.opengl.org/registry/specs/EXT/texture_env_combine.txt
#define GL_APPLE_specular_vector           0 // 159 [inc] http://www.opengl.org/registry/specs/APPLE/specular_vector.txt
#define GL_APPLE_transform_hint            0 // 160 [inc] http://www.opengl.org/registry/specs/APPLE/transform_hint.txt
//#define GL_SUNX_constant_data              0 // 163 [] http://www.opengl.org/registry/specs/SUNX/constant_data.txt
//#define GL_SUN_global_alpha                0 // 164 [] http://www.opengl.org/registry/specs/SUN/global_alpha.txt
//#define GL_SUN_triangle_list               0 // 165 [] http://www.opengl.org/registry/specs/SUN/triangle_list.txt
//#define GL_SUN_vertex                      0 // 166 [] http://www.opengl.org/registry/specs/SUN/vertex.txt
#define WGL_EXT_display_color_table        0 // 167 [inc] http://www.opengl.org/registry/specs/EXT/wgl_display_color_table.txt
//#define WGL_EXT_extensions_string          0 // 168 [] http://www.opengl.org/registry/specs/EXT/wgl_extensions_string.txt
//#define WGL_EXT_make_current_read          0 // 169 [] http://www.opengl.org/registry/specs/EXT/wgl_make_current_read.txt
//#define WGL_EXT_pixel_format               0 // 170 [] http://www.opengl.org/registry/specs/EXT/wgl_pixel_format.txt
//#define WGL_EXT_pbuffer                    0 // 171 [] http://www.opengl.org/registry/specs/EXT/wgl_pbuffer.txt
//#define WGL_EXT_swap_control               0 // 172 [] http://www.opengl.org/registry/specs/EXT/wgl_swap_control.txt
//#define GL_EXT_blend_func_separate         0 // 173 [] http://www.opengl.org/registry/specs/EXT/blend_func_separate.txt
//#define GL_INGR_color_clamp                0 // 174 [] http://www.opengl.org/registry/specs/INGR/color_clamp.txt
//#define GL_INGR_interlace_read             0 // 175 [] http://www.opengl.org/registry/specs/INGR/interlace_read.txt
//#define GL_EXT_stencil_wrap                0 // 176 [] http://www.opengl.org/registry/specs/EXT/stencil_wrap.txt
//#define WGL_EXT_depth_float                0 // 177 [] http://www.opengl.org/registry/specs/EXT/wgl_depth_float.txt
//#define GL_EXT_422_pixels                  0 // 178 [] http://www.opengl.org/registry/specs/EXT/422_pixels.txt
//#define GL_NV_texgen_reflection            0 // 179 [] http://www.opengl.org/registry/specs/NV/texgen_reflection.txt
//#define GL_SGIX_texture_range              0 // 181 [] http://www.opengl.org/registry/specs/SGIX/texture_range.txt
//#define GL_SUN_convolution_border_modes    0 // 182 [] http://www.opengl.org/registry/specs/SUN/convolution_border_modes.txt
//#define GLX_SUN_get_transparent_index      0 // 183 [] http://www.opengl.org/registry/specs/SUN/get_transparent_index.txt
//#define GL_EXT_texture_env_add             0 // 185 [] http://www.opengl.org/registry/specs/EXT/texture_env_add.txt
//#define GL_EXT_texture_lod_bias            0 // 186 [] http://www.opengl.org/registry/specs/EXT/texture_lod_bias.txt
//#define GL_EXT_texture_filter_anisotropic  0 // 187 [] http://www.opengl.org/registry/specs/EXT/texture_filter_anisotropic.txt
//#define GL_EXT_vertex_weighting            0 // 188 [] http://www.opengl.org/registry/specs/EXT/vertex_weighting.txt
//#define GL_NV_light_max_exponent           0 // 189 [] http://www.opengl.org/registry/specs/NV/light_max_exponent.txt
//#define GL_NV_vertex_array_range           0 // 190 [] http://www.opengl.org/registry/specs/NV/vertex_array_range.txt
//#define GL_NV_register_combiners           0 // 191 [] http://www.opengl.org/registry/specs/NV/register_combiners.txt
//#define GL_NV_fog_distance                 0 // 192 [] http://www.opengl.org/registry/specs/NV/fog_distance.txt
//#define GL_NV_texgen_emboss                0 // 193 [] http://www.opengl.org/registry/specs/NV/texgen_emboss.txt
//#define GL_NV_blend_square                 0 // 194 [] http://www.opengl.org/registry/specs/NV/blend_square.txt
//#define GL_NV_texture_env_combine4         0 // 195 [] http://www.opengl.org/registry/specs/NV/texture_env_combine4.txt
//#define GL_MESA_resize_buffers             0 // 196 [] http://www.opengl.org/registry/specs/MESA/resize_buffers.txt
//#define GL_MESA_window_pos                 0 // 197 [] http://www.opengl.org/registry/specs/MESA/window_pos.txt
//#define GL_EXT_texture_compression_s3tc    0 // 198 [] http://www.opengl.org/registry/specs/EXT/texture_compression_s3tc.txt
//#define GL_IBM_cull_vertex                 0 // 199 [] http://www.opengl.org/registry/specs/IBM/cull_vertex.txt
//#define GL_IBM_multimode_draw_arrays       0 // 200 [] http://www.opengl.org/registry/specs/IBM/multimode_draw_arrays.txt
//#define GL_IBM_vertex_array_lists          0 // 201 [] http://www.opengl.org/registry/specs/IBM/vertex_array_lists.txt
//#define GL_3DFX_texture_compression_FXT1   0 // 206 [] http://www.opengl.org/registry/specs/3DFX/texture_compression_FXT1.txt
//#define GL_3DFX_multisample                0 // 207 [] http://www.opengl.org/registry/specs/3DFX/3dfx_multisample.txt
//#define GLX_3DFX_multisample               0 // ^207
//#define WGL_3DFX_multisample               0 // ^207
//#define GL_3DFX_tbuffer                    0 //  208 [] http://www.opengl.org/registry/specs/3DFX/tbuffer.txt
//#define GL_EXT_multisample                 0 // 209 [] http://www.opengl.org/registry/specs/EXT/wgl_multisample.txt
//#define WGL_EXT_multisample                0 // ^209 
//#define GL_SGIX_vertex_preclip             0 // 210 [] http://www.opengl.org/registry/specs/SGIX/vertex_preclip.txt
//#define GL_SGIX_vertex_preclip_hint        0 // ^210
#define GL_SGIX_convolution_accuracy       0 // 211 [inc] http://www.opengl.org/registry/specs/SGIX/convolution_accuracy.txt
#define GL_SGIX_resample                   0 // 212 [inc] http://www.opengl.org/registry/specs/SGIX/resample.txt
//#define GL_SGIS_point_line_texgen          0 // 213 [] http://www.opengl.org/registry/specs/SGIS/point_line_texgen.txt
//#define GL_SGIS_texture_color_mask         0 // 214 [] http://www.opengl.org/registry/specs/SGIS/texture_color_mask.txt
//#define GLX_MESA_copy_sub_buffer           0 // 215 [] http://www.opengl.org/registry/specs/MESA/copy_sub_buffer.txt
//#define GLX_MESA_pixmap_colormap           0 // 216 [] http://www.opengl.org/registry/specs/MESA/pixmap_colormap.txt
//#define GLX_MESA_release_buffers           0 // 217 [] http://www.opengl.org/registry/specs/MESA/release_buffers.txt
//#define GLX_MESA_set_3dfx_mode             0 // 218 [] http://www.opengl.org/registry/specs/MESA/set_3dfx_mode.txt
//#define GL_EXT_texture_env_dot3            0 // 220 [] http://www.opengl.org/registry/specs/EXT/texture_env_dot3.txt
//#define GL_ATI_texture_mirror_once         0 // 221 [] http://www.opengl.org/registry/specs/ATI/texture_mirror_once.txt
//#define GL_NV_fence                        0 // 222 [] http://www.opengl.org/registry/specs/NV/fence.txt
//#define GL_IBM_static_data                 0 // 223 [] http://www.opengl.org/registry/specs/IBM/static_data.txt
//#define GL_IBM_texture_mirrored_repeat     0 // 224 [] http://www.opengl.org/registry/specs/IBM/texture_mirrored_repeat.txt
//#define GL_NV_evaluators                   0 // 225 [] http://www.opengl.org/registry/specs/NV/evaluators.txt
//#define GL_NV_packed_depth_stencil         0 // 226 [] http://www.opengl.org/registry/specs/NV/packed_depth_stencil.txt
//#define GL_NV_register_combiners2          0 // 227 [] http://www.opengl.org/registry/specs/NV/register_combiners2.txt
//#define GL_NV_texture_compression_vtc      0 // 228 [] http://www.opengl.org/registry/specs/NV/texture_compression_vtc.txt
//#define GL_NV_texture_rectangle            0 // 229 [] http://www.opengl.org/registry/specs/NV/texture_rectangle.txt
//#define GL_NV_texture_shader               0 // 230 [] http://www.opengl.org/registry/specs/NV/texture_shader.txt
//#define GL_NV_texture_shader2              0 // 231 [] http://www.opengl.org/registry/specs/NV/texture_shader2.txt
//#define GL_NV_vertex_array_range2          0 // 232 [] http://www.opengl.org/registry/specs/NV/vertex_array_range2.txt
//#define GL_NV_vertex_program               0 // 233 [] http://www.opengl.org/registry/specs/NV/vertex_program.txt
//#define GLX_SGIX_visual_select_group       0 // 234 [] http://www.opengl.org/registry/specs/SGIX/visual_select_group.txt
//#define GL_SGIX_texture_coordinate_clamp   0 // 235 [] http://www.opengl.org/registry/specs/SGIX/texture_coordinate_clamp.txt
//#define GLX_OML_swap_method                0 // 237 [] http://www.opengl.org/registry/specs/OML/glx_swap_method.txt
//#define GLX_OML_sync_control               0 // 238 [] http://www.opengl.org/registry/specs/OML/glx_sync_control.txt
//#define GL_OML_interlace                   0 // 239 [] http://www.opengl.org/registry/specs/OML/interlace.txt
//#define GL_OML_subsample                   0 // 240 [] http://www.opengl.org/registry/specs/OML/subsample.txt
//#define GL_OML_resample                    0 // 241 [] http://www.opengl.org/registry/specs/OML/resample.txt
//#define WGL_OML_sync_control               0 // 242 [] http://www.opengl.org/registry/specs/OML/wgl_sync_control.txt
//#define GL_NV_copy_depth_to_color          0 // 243 [] http://www.opengl.org/registry/specs/NV/copy_depth_to_color.txt
//#define GL_ATI_envmap_bumpmap              0 // 244 [] http://www.opengl.org/registry/specs/ATI/envmap_bumpmap.txt
//#define GL_ATI_fragment_shader             0 // 245 [] http://www.opengl.org/registry/specs/ATI/fragment_shader.txt
//#define GL_ATI_pn_triangles                0 // 246 [] http://www.opengl.org/registry/specs/ATI/pn_triangles.txt
//#define GL_ATI_vertex_array_object         0 // 247 [] http://www.opengl.org/registry/specs/ATI/vertex_array_object.txt
//#define GL_EXT_vertex_shader               0 // 248 [] http://www.opengl.org/registry/specs/EXT/vertex_shader.txt
//#define GL_ATI_vertex_streams              0 // 249 [] http://www.opengl.org/registry/specs/ATI/vertex_streams.txt
//#define WGL_I3D_digital_video_control      0 // 250 [] http://www.opengl.org/registry/specs/I3D/wgl_digital_video_control.txt
//#define WGL_I3D_gamma                      0 // 251 [] http://www.opengl.org/registry/specs/I3D/wgl_gamma.txt
//#define WGL_I3D_genlock                    0 // 252 [] http://www.opengl.org/registry/specs/I3D/wgl_genlock.txt
//#define WGL_I3D_image_buffer               0 // 253 [] http://www.opengl.org/registry/specs/I3D/wgl_image_buffer.txt
//#define WGL_I3D_swap_frame_lock            0 // 254 [] http://www.opengl.org/registry/specs/I3D/wgl_swap_frame_lock.txt
//#define WGL_I3D_swap_frame_usage           0 // 255 [] http://www.opengl.org/registry/specs/I3D/wgl_swap_frame_usage.txt
//#define GL_ATI_element_array               0 // 256 [] http://www.opengl.org/registry/specs/ATI/element_array.txt
//#define GL_SUN_mesh_array                  0 // 257 [] http://www.opengl.org/registry/specs/SUN/mesh_array.txt
//#define GL_SUN_slice_accum                 0 // 258 [] http://www.opengl.org/registry/specs/SUN/slice_accum.txt
//#define GL_NV_multisample_filter_hint      0 // 259 [] http://www.opengl.org/registry/specs/NV/multisample_filter_hint.txt
//#define GL_NV_depth_clamp                  0 // 260 [] http://www.opengl.org/registry/specs/NV/depth_clamp.txt
//#define GL_NV_occlusion_query              0 // 261 [] http://www.opengl.org/registry/specs/NV/occlusion_query.txt
//#define GL_NV_point_sprite                 0 // 262 [] http://www.opengl.org/registry/specs/NV/point_sprite.txt
//#define WGL_NV_render_depth_texture        0 // 263 [] http://www.opengl.org/registry/specs/NV/render_depth_texture.txt
//#define WGL_NV_render_texture_rectangle    0 // 264 [] http://www.opengl.org/registry/specs/NV/render_texture_rectangle.txt
//#define GL_NV_texture_shader3              0 // 265 [] http://www.opengl.org/registry/specs/NV/texture_shader3.txt
//#define GL_NV_vertex_program1_1            0 // 266 [] http://www.opengl.org/registry/specs/NV/texture_shader3.txt
//#define GL_EXT_shadow_funcs                0 // 267 [] http://www.opengl.org/registry/specs/EXT/shadow_funcs.txt
//#define GL_EXT_stencil_two_side            0 // 268 [] http://www.opengl.org/registry/specs/EXT/stencil_two_side.txt
//#define GL_ATI_text_fragment_shader        0 // 269 [] http://www.opengl.org/registry/specs/ATI/text_fragment_shader.txt
//#define GL_APPLE_client_storage            0 // 270 [] http://www.opengl.org/registry/specs/APPLE/client_storage.txt
//#define GL_APPLE_element_array             0 // 271 [] http://www.opengl.org/registry/specs/APPLE/element_array.txt
//#define GL_APPLE_fence                     0 // 272 [] http://www.opengl.org/registry/specs/APPLE/fence.txt
//#define GL_APPLE_vertex_array_object       0 // 273 [] http://www.opengl.org/registry/specs/APPLE/vertex_array_object.txt
//#define GL_APPLE_vertex_array_range        0 // 274 [] http://www.opengl.org/registry/specs/APPLE/vertex_array_range.txt
//#define GL_APPLE_ycbcr_422                 0 // 275 [] http://www.opengl.org/registry/specs/APPLE/ycbcr_422.txt
#define GL_S3_s3tc                         0 // 276 [inc] http://www.opengl.org/registry/specs/S3/s3tc.txt
//#define GL_ATI_draw_buffers                0 // 277 [] http://www.opengl.org/registry/specs/ATI/draw_buffers.txt
//#define WGL_ATI_pixel_format_float         0 // 278 [] http://www.opengl.org/registry/specs/ATI/pixel_format_float.txt
//#define GL_ATI_texture_env_combine3        0 // 279 [] http://www.opengl.org/registry/specs/ATI/texture_env_combine3.txt
//#define GL_ATI_texture_float               0 // 280 [] http://www.opengl.org/registry/specs/ATI/texture_float.txt
//#define GL_NV_float_buffer                 0 // 281 [] http://www.opengl.org/registry/specs/NV/float_buffer.txt
//#define WGL_NV_float_buffer                0 // ^281
//#define GLX_NV_float_buffer                0 // ^281
//#define GL_NV_fragment_program             0 // 282 [] http://www.opengl.org/registry/specs/NV/fragment_program.txt
//#define GL_NV_half_float                   0 // 283 [] http://www.opengl.org/registry/specs/NV/half_float.txt
//#define GL_NV_pixel_data_range             0 // 284 [] http://www.opengl.org/registry/specs/NV/pixel_data_range.txt
//#define GL_NV_primitive_restart            0 // 285 [] http://www.opengl.org/registry/specs/NV/primitive_restart.txt
//#define GL_NV_texture_expand_normal        0 // 286 [] http://www.opengl.org/registry/specs/NV/texture_expand_normal.txt
//#define GL_NV_vertex_program2              0 // 287 [] http://www.opengl.org/registry/specs/NV/vertex_program2.txt
//#define GL_ATI_map_object_buffer           0 // 288 [] http://www.opengl.org/registry/specs/ATI/map_object_buffer.txt
//#define GL_ATI_separate_stencil            0 // 289 [] http://www.opengl.org/registry/specs/ATI/separate_stencil.txt
//#define GL_ATI_vertex_attrib_array_object  0 // 290 [] http://www.opengl.org/registry/specs/ATI/vertex_attrib_array_object.txt
//#define GL_OES_byte_coordinates            0 // 291 [] THIS IS DISABLED UNTIL THEY FIGURE OUT HOW MANY PARAMETERS THEIR FUNCS HAVE http://www.opengl.org/registry/specs/OES/OES_byte_coordinates.txt
//#define GL_OES_fixed_point                 0 // 292 [] http://www.opengl.org/registry/specs/OES/OES_fixed_point.txt
//#define GL_OES_single_precision            0 // 293 [] http://www.opengl.org/registry/specs/OES/OES_single_precision.txt
//#define GL_OES_compressed_paletted_texture 0 // 294 [] http://www.opengl.org/registry/specs/OES/OES_compressed_paletted_texture.txt
//#define GL_OES_read_format                 0 // 295 [] http://www.opengl.org/registry/specs/OES/OES_read_format.txt
//#define GL_OES_query_matrix                0 // 296 [] http://www.opengl.org/registry/specs/OES/OES_query_matrix.txt
//#define GL_EXT_depth_bounds_test           0 // 297 [] http://www.opengl.org/registry/specs/EXT/depth_bounds_test.txt
//#define GL_EXT_texture_mirror_clamp        0 // 298 [] http://www.opengl.org/registry/specs/EXT/texture_mirror_clamp.txt
//#define GL_EXT_blend_equation_separate     0 // 299 [] http://www.opengl.org/registry/specs/EXT/blend_equation_separate.txt
//#define GL_MESA_pack_invert                0 // 300 [] http://www.opengl.org/registry/specs/MESA/pack_invert.txt
//#define GL_MESA_ycbcr_texture              0 // 301 [] http://www.opengl.org/registry/specs/MESA/ycbcr_texture.txt
//#define GL_EXT_pixel_buffer_object         0 // 302 [] http://www.opengl.org/registry/specs/EXT/pixel_buffer_object.txt
//#define GL_NV_fragment_program_option      0 // 303 [] http://www.opengl.org/registry/specs/NV/fragment_program_option.txt
//#define GL_NV_fragment_program2            0 // 304 [] http://www.opengl.org/registry/specs/NV/fragment_program2.txt
//#define GL_NV_vertex_program2_option       0 // 305 [] http://www.opengl.org/registry/specs/NV/vertex_program2_option.txt
//#define GL_NV_vertex_program3              0 // 306 [] http://www.opengl.org/registry/specs/NV/vertex_program3.txt
//#define GLX_SGIX_hyperpipe                 0 // 307 [] http://www.opengl.org/registry/specs/SGIX/hyperpipe_group.txt
//#define GLX_MESA_agp_offset                0 // 308 [] http://www.opengl.org/registry/specs/MESA/agp_offset.txt
//#define GL_EXT_texture_compression_dxt1    0 // 309 [] http://www.opengl.org/registry/specs/EXT/texture_compression_dxt1.txt
//#define GL_EXT_framebuffer_object          0 // 310 [] http://www.opengl.org/registry/specs/EXT/framebuffer_object.txt
//#define GL_GREMEDY_string_marker           0 // 311 [] http://www.opengl.org/registry/specs/GREMEDY/string_marker.txt
//#define GL_EXT_packed_depth_stencil        0 // 312 [] http://www.opengl.org/registry/specs/EXT/packed_depth_stencil.txt
//#define WGL_3DL_stereo_control             0 // 313 [] http://www.opengl.org/registry/specs/3DL/stereo_control.txt
//#define GL_EXT_stencil_clear_tag           0 // 314 [] http://www.opengl.org/registry/specs/EXT/stencil_clear_tag.txt
//#define GL_EXT_texture_sRGB                0 // 315 [] http://www.opengl.org/registry/specs/EXT/texture_sRGB.txt
//#define GL_EXT_framebuffer_blit            0 // 316 [] http://www.opengl.org/registry/specs/EXT/framebuffer_blit.txt
//#define GL_EXT_framebuffer_multisample     0 // 317 [] http://www.opengl.org/registry/specs/EXT/framebuffer_multisample.txt
//#define GL_MESAX_texture_stack             0 // 318 [] http://www.opengl.org/registry/specs/MESAX/texture_stack.txt
//#define GL_EXT_timer_query                 0 // 319 [] http://www.opengl.org/registry/specs/EXT/timer_query.txt
//#define GL_EXT_gpu_program_parameters      0 // 320 [] http://www.opengl.org/registry/specs/EXT/gpu_program_parameters.txt
//#define GL_APPLE_flush_buffer_range        0 // 321 [] http://www.opengl.org/registry/specs/APPLE/flush_buffer_range.txt
//#define GL_NV_gpu_program4                 0 // 322 [] http://www.opengl.org/registry/specs/NV/gpu_program4.txt
//#define GL_NV_geometry_program4            0 // 323 [] http://www.opengl.org/registry/specs/NV/geometry_program4.txt
//#define GL_EXT_geometry_shader4            0 // 324 [] http://www.opengl.org/registry/specs/EXT/geometry_shader4.txt
//#define GL_NV_vertex_program4              0 // 325 [] http://www.opengl.org/registry/specs/NV/vertex_program4.txt
//#define GL_EXT_gpu_shader4                 0 // 326 [] http://www.opengl.org/registry/specs/EXT/gpu_shader4.txt
//#define GL_EXT_draw_instanced              0 // 327 [] http://www.opengl.org/registry/specs/EXT/draw_instanced.txt
//#define GL_EXT_packed_float                0 // 328 [] http://www.opengl.org/registry/specs/EXT/packed_float.txt
//#define WGL_EXT_pixel_format_packed_float  0 // ^328
//#define GLX_EXT_fbconfig_packed_float      0 // ^328
//#define GL_EXT_texture_array               0 // 329 [] http://www.opengl.org/registry/specs/EXT/texture_array.txt
//#define GL_EXT_texture_buffer_object       0 // 330 [] http://www.opengl.org/registry/specs/EXT/texture_buffer_object.txt
//#define GL_EXT_texture_compression_latc    0 // 331 [legacy] http://www.opengl.org/registry/specs/EXT/texture_compression_latc.txt
//#define GL_NV_texture_compression_latc     0 // ^331
//#define GL_EXT_texture_compression_rgtc    0 // 332 [] http://www.opengl.org/registry/specs/EXT/texture_compression_rgtc.txt
//#define GL_EXT_texture_shared_exponent     0 // 333 [] http://www.opengl.org/registry/specs/EXT/texture_shared_exponent.txt
//#define GL_NV_depth_buffer_float           0 // 334 [] http://www.opengl.org/registry/specs/NV/depth_buffer_float.txt
//#define GL_NV_fragment_program4            0 // 335 [] http://www.opengl.org/registry/specs/NV/fragment_program4.txt
//#define GL_NV_framebuffer_multisample_coverage 0 // 336 [] http://www.opengl.org/registry/specs/NV/framebuffer_multisample_coverage.txt
//#define GL_EXT_framebuffer_sRGB            0 // 337 [] http://www.opengl.org/registry/specs/EXT/framebuffer_sRGB.txt
//#define GLX_EXT_framebuffer_sRGB           0 // ^337
//#define WGL_EXT_framebuffer_sRGB           0 // ^337
//#define GL_NV_geometry_shader4             0 // 338 [] http://www.opengl.org/registry/specs/NV/geometry_shader4.txt
//#define GL_NV_parameter_buffer_object      0 // 339 [] http://www.opengl.org/registry/specs/NV/parameter_buffer_object.txt
//#define GL_EXT_draw_buffers2               0 // 340 [] http://www.opengl.org/registry/specs/EXT/draw_buffers2.txt
//#define GL_NV_transform_feedback           0 // 341 [] http://www.opengl.org/registry/specs/NV/transform_feedback.txt
//#define GL_EXT_bindable_uniform            0 // 342 [] http://www.opengl.org/registry/specs/EXT/bindable_uniform.txt
//#define GL_EXT_texture_integer             0 // 343 [] http://www.opengl.org/registry/specs/EXT/texture_integer.txt
//#define GLX_EXT_texture_from_pixmap        0 // 344 [] http://www.opengl.org/registry/specs/EXT/texture_from_pixmap.txt
//#define GL_GREMEDY_frame_terminator        0 // 345 [] http://www.opengl.org/registry/specs/GREMEDY/frame_terminator.txt
//#define GL_NV_conditional_render           0 // 346 [] http://www.opengl.org/registry/specs/NV/conditional_render.txt
//#define GL_NV_present_video                0 // 347 [] http://www.opengl.org/registry/specs/NV/present_video.txt
//#define GLX_NV_present_video               0 // ^347
//#define WGL_NV_present_video               0 // ^347
//#define GLX_NV_video_out                   0 // 348 [] http://www.opengl.org/registry/specs/NV/glx_video_output.txt
//#define WGL_NV_video_output                0 // 349 [] http://www.opengl.org/registry/specs/NV/wgl_video_output.txt
//#define GLX_NV_swap_group                  0 // 350 [] http://www.opengl.org/registry/specs/NV/glx_swap_group.txt
//#define WGL_NV_swap_group                  0 // 351 [] http://www.opengl.org/registry/specs/NV/wgl_swap_group.txt
//#define GL_EXT_transform_feedback          0 // 352 [] http://www.opengl.org/registry/specs/EXT/transform_feedback.txt
//#define GL_EXT_direct_state_access         0 // 353 [] http://www.opengl.org/registry/specs/EXT/direct_state_access.txt
//#define GL_EXT_vertex_array_bgra           0 // 354 [] http://www.opengl.org/registry/specs/EXT/vertex_array_bgra.txt
//#define WGL_NV_gpu_affinity                0 // 355 [] http://www.opengl.org/registry/specs/NV/gpu_affinity.txt
//#define GL_EXT_texture_swizzle             0 // 356 [] http://www.opengl.org/registry/specs/EXT/texture_swizzle.txt
//#define GL_NV_explicit_multisample         0 // 357 [] http://www.opengl.org/registry/specs/NV/explicit_multisample.txt
//#define GL_NV_transform_feedback2          0 // 358 [] http://www.opengl.org/registry/specs/NV/transform_feedback2.txt
//#define GL_ATI_meminfo                     0 // 359 [] http://www.opengl.org/registry/specs/ATI/meminfo.txt
//#define GL_AMD_performance_monitor         0 // 360 [] http://www.opengl.org/registry/specs/AMD/performance_monitor.txt
//#define WGL_AMD_gpu_association            0 // 361 [] http://www.opengl.org/registry/specs/AMD/wgl_gpu_association.txt
//#define GL_AMD_texture_texture4            0 // 362 [] http://www.opengl.org/registry/specs/AMD/texture_texture4.txt
//#define GL_AMD_vertex_shader_tessellator   0 // 363 [] http://www.opengl.org/registry/specs/AMD/vertex_shader_tessellator.txt
//#define GL_EXT_provoking_vertex            0 // 364 [] http://www.opengl.org/registry/specs/EXT/provoking_vertex.txt
//#define GL_EXT_texture_snorm               0 // 365 [] http://www.opengl.org/registry/specs/EXT/texture_snorm.txt
//#define GL_AMD_draw_buffers_blend          0 // 366 [] http://www.opengl.org/registry/specs/AMD/draw_buffers_blend.txt
//#define GL_APPLE_texture_range             0 // 367 [] http://www.opengl.org/registry/specs/APPLE/texture_range.txt
//#define GL_APPLE_float_pixels              0 // 368 [] http://www.opengl.org/registry/specs/APPLE/float_pixels.txt
//#define GL_APPLE_vertex_program_evaluators 0 // 369 [] http://www.opengl.org/registry/specs/APPLE/vertex_program_evaluators.txt
//#define GL_APPLE_aux_depth_stencil         0 // 370 [] http://www.opengl.org/registry/specs/APPLE/aux_depth_stencil.txt
//#define GL_APPLE_object_purgeable          0 // 371 [] http://www.opengl.org/registry/specs/APPLE/object_purgeable.txt
//#define GL_APPLE_row_bytes                 0 // 372 [] http://www.opengl.org/registry/specs/APPLE/row_bytes.txt
//#define GL_APPLE_rgb_422                   0 // 373 [] http://www.opengl.org/registry/specs/APPLE/rgb_422.txt
//#define GL_NV_video_capture                0 // 374 [] http://www.opengl.org/registry/specs/NV/video_capture.txt
//#define GLX_NV_video_capture               0 // ^374
//#define WGL_NV_video_capture               0 // ^374
//#define GLX_EXT_swap_control               0 // 375 [] http://www.opengl.org/registry/specs/EXT/swap_control.txt
//#define GL_NV_copy_image                   0 // 376 [] http://www.opengl.org/registry/specs/NV/copy_image.txt
//#define WGL_NV_copy_image                  0 // ^376
//#define GLX_NV_copy_image                  0 // ^376
//#define GL_EXT_separate_shader_objects     0 // 377 [] http://www.opengl.org/registry/specs/EXT/separate_shader_objects.txt
//#define GL_NV_parameter_buffer_object2     0 // 378 [] http://www.opengl.org/registry/specs/NV/parameter_buffer_object2.txt
//#define GL_NV_shader_buffer_load           0 // 379 [] http://www.opengl.org/registry/specs/NV/shader_buffer_load.txt
//#define GL_NV_vertex_buffer_unified_memory 0 // 380 [] http://www.opengl.org/registry/specs/NV/vertex_buffer_unified_memory.txt
//#define GL_NV_texture_barrier              0 // 381 [] http://www.opengl.org/registry/specs/NV/texture_barrier.txt
//#define GL_AMD_shader_stencil_export       0 // 382 [] http://www.opengl.org/registry/specs/AMD/shader_stencil_export.txt
//#define GL_AMD_seamless_cubemap_per_texture 0 // 383 [] http://www.opengl.org/registry/specs/AMD/seamless_cubemap_per_texture.txt
//#define GLX_INTEL_swap_event               0 // 384 [] http://www.opengl.org/registry/specs/INTEL/swap_event.txt
//#define GL_AMD_conservative_depth          0 // 385 [] http://www.opengl.org/registry/specs/AMD/conservative_depth.txt
//#define GL_EXT_shader_image_load_store     0 // 386 [] http://www.opengl.org/registry/specs/EXT/shader_image_load_store.txt
//#define GL_EXT_vertex_attrib_64bit         0 // 387 [] http://www.opengl.org/registry/specs/EXT/vertex_attrib_64bit.txt
//#define GL_NV_gpu_program5                 0 // 388 []
//#define GL_NV_gpu_program_fp64             0 // http://www.opengl.org/registry/specs/NV/gpu_program5.txt
//#define GL_NV_gpu_shader5                  0 // 389 [] http://www.opengl.org/registry/specs/NV/gpu_shader5.txt
//#define GL_NV_shader_buffer_store          0 // 390 [] http://www.opengl.org/registry/specs/NV/shader_buffer_store.txt
//#define GL_NV_tessellation_program5        0 // 391 [] http://www.opengl.org/registry/specs/NV/tessellation_program5.txt
//#define GL_NV_vertex_attrib_integer_64bit  0 // 392 [] http://www.opengl.org/registry/specs/NV/vertex_attrib_integer_64bit.txt
//#define GL_NV_multisample_coverage         0 // 393 [] http://www.opengl.org/registry/specs/NV/multisample_coverage.txt
//#define GLX_NV_multisample_coverage        0 // ^393
//#define WGL_NV_multisample_coverage        0 // ^393
//#define GL_AMD_name_gen_delete             0 // 394 [] http://www.opengl.org/registry/specs/AMD/name_gen_delete.txt
//#define GL_AMD_debug_output                0 // 395 [] http://www.opengl.org/registry/specs/AMD/debug_output.txt
//#define GL_NV_vdpau_interop                0 // 396 [] http://www.opengl.org/registry/specs/NV/vdpau_interop.txt
//#define GL_AMD_transform_feedback3_lines_triangles 0 // 397 [] http://www.opengl.org/registry/specs/AMD/transform_feedback3_lines_triangles.txt
//#define GLX_AMD_gpu_association            0 // 398 [] http://www.opengl.org/registry/specs/AMD/glx_gpu_association.txt
//#define GLX_EXT_create_context_es_profile  0 // 399 [] http://www.opengl.org/registry/specs/EXT/glx_create_context_es2_profile.txt
//#define GLX_EXT_create_context_es2_profile 0 // ^399
//#define WGL_EXT_create_context_es_profile  0 // 400 [] http://www.opengl.org/registry/specs/EXT/wgl_create_context_es2_profile.txt
//#define WGL_EXT_create_context_es2_profile 0 // ^400
//#define GL_AMD_depth_clamp_separate        0 // 401 [] http://www.opengl.org/registry/specs/AMD/depth_clamp_separate.txt
//#define GL_EXT_texture_sRGB_decode         0 // 402 [] http://www.opengl.org/registry/specs/EXT/texture_sRGB_decode.txt
//#define GL_NV_texture_multisample          0 // 403 [] GL_ missing in registry, asuming mistake http://www.opengl.org/registry/specs/NV/texture_multisample.txt
//#define GL_AMD_blend_minmax_factor         0 // 404 [] http://www.opengl.org/registry/specs/AMD/blend_minmax_factor.txt
//#define GL_AMD_sample_positions            0 // 405 [] http://www.opengl.org/registry/specs/AMD/sample_positions.txt
//#define GL_EXT_x11_sync_object             0 // 406 [] http://www.opengl.org/registry/specs/EXT/x11_sync_object.txt
//#define WGL_NV_DX_interop                  0 // 407 [] http://www.opengl.org/registry/specs/NV/DX_interop.txt
//#define GL_AMD_multi_draw_indirect         0 // 408 [] http://www.opengl.org/registry/specs/AMD/multi_draw_indirect.txt
//#define GL_EXT_framebuffer_multisample_blit_scaled 0 // 409 [] http://www.opengl.org/registry/specs/EXT/framebuffer_multisample_blit_scaled.txt
//#define GL_NV_path_rendering               0 // 410 [] http://www.opengl.org/registry/specs/NV/path_rendering.txt
//#define GL_AMD_pinned_memory               0 // 411 [] http://www.opengl.org/registry/specs/AMD/pinned_memory.txt
//#define WGL_NV_DX_interop2                 0 // 412 [] http://www.opengl.org/registry/specs/NV/DX_interop2.txt
//#define GL_AMD_stencil_operation_extended  0 // 413 [] http://www.opengl.org/registry/specs/AMD/stencil_operation_extended.txt
//#define GLX_EXT_swap_control_tear          0 // 414 [] http://www.opengl.org/registry/specs/EXT/glx_swap_control_tear.txt
//#define WGL_EXT_swap_control_tear          0 // 415 [] http://www.opengl.org/registry/specs/EXT/wgl_swap_control_tear.txt
//#define GL_AMD_vertex_shader_viewport_index 0 // 416 [] http://www.opengl.org/registry/specs/AMD/vertex_shader_viewport_index.txt
//#define GL_AMD_vertex_shader_layer         0 // 417 [] http://www.opengl.org/registry/specs/AMD/vertex_shader_layer.txt
//#define GL_NV_bindless_texture             0 // 418 [] http://www.opengl.org/registry/specs/NV/bindless_texture.txt
//#define GL_NV_shader_atomic_float          0 // 419 [] http://www.opengl.org/registry/specs/NV/shader_atomic_float.txt
//#define GL_AMD_query_buffer_object         0 // 420 [] http://www.opengl.org/registry/specs/AMD/query_buffer_object.txt
//#define GL_NV_compute_program5             0 // 421 [] http://www.opengl.org/registry/specs/NV/compute_program5.txt
//#define GL_NV_shader_storage_buffer_object 0 // 422 [] http://www.opengl.org/registry/specs/NV/shader_storage_buffer_object.txt
//#define GL_NV_shader_atomic_counters       0 // 423 [] http://www.opengl.org/registry/specs/NV/shader_atomic_counters.txt
//#define GL_NV_deep_texture3D               0 // 424 [] http://www.opengl.org/registry/specs/NV/deep_texture3D.txt
//#define GL_NVX_conditional_render          0 // 425 [] http://www.opengl.org/registry/specs/NVX/nvx_conditional_render.txt
//#define GL_AMD_sparse_texture              0 // 426 [] http://www.opengl.org/registry/specs/AMD/sparse_texture.txt
//#define GLX_EXT_buffer_age                 0 // 427 [] http://www.opengl.org/registry/specs/EXT/glx_buffer_age.txt
//#define GL_AMD_shader_trinary_minmax       0 // 428 [] http://www.opengl.org/registry/specs/AMD/shader_trinary_minmax.txt
//#define GL_INTEL_map_texture               0 // 429 [] http://www.opengl.org/registry/specs/INTEL/map_texture.txt
//#define GL_NV_draw_texture                 0 // 430 [] http://www.opengl.org/registry/specs/NV/draw_texture.txt
//#define GL_AMD_interleaved_elements        0 // 431[] http://www.opengl.org/registry/specs/AMD/interleaved_elements.txt
//#define GL_NV_bindless_multi_draw_indirect 0 // 432 [] http://www.opengl.org/registry/specs/NV/bindless_multi_draw_indirect.txt
//#define GL_NV_blend_equation_advanced      0 // 433 [] http://www.opengl.org/registry/specs/NV/blend_equation_advanced.txt
//#define GL_NV_blend_equation_advanced_coherent 0 // ^433
//#define GL_NV_gpu_program5_mem_extended    0 // 434 [] http://www.opengl.org/registry/specs/NV/gpu_program5_mem_extended.txt
//#define GL_AMD_shader_atomic_counter_ops   0 // 435 [] http://www.opengl.org/registry/specs/AMD/shader_atomic_counter_ops.txt
//#define WGL_NV_delay_before_swap           0 // 436 [] http://www.opengl.org/registry/specs/NV/wgl_delay_before_swap.txt
//#define GL_EXT_shader_integer_mix          0 // 437 [] http://www.opengl.org/registry/specs/EXT/shader_integer_mix.txt
//#define GL_NVX_gpu_memory_info             0 // 438 [] http://www.opengl.org/registry/specs/NVX/gpu_memory_info.txt
//#define GL_EXT_debug_label                 0 // 439 [] http://www.opengl.org/registry/specs/EXT/EXT_debug_label.txt
//#define GL_EXT_debug_marker                0 // 440 [] http://www.opengl.org/registry/specs/EXT/EXT_debug_marker.txt
//#define GL_INTEL_fragment_shader_ordering  0 // 441 [] http://www.opengl.org/registry/specs/INTEL/fragment_shader_ordering.txt
//#define GL_AMD_occlusion_query_event       0 // 442 [] http://www.opengl.org/registry/specs/AMD/occlusion_query_event.txt
//#define GL_INTEL_performance_query         0 // 443 [] DISABLED http://www.opengl.org/registry/specs/INTEL/performance_query.txt
//#define GL_AMD_shader_stencil_value_export 0 // 444 [] http://www.opengl.org/registry/specs/AMD/shader_stencil_value_export.txt
//#define GLX_NV_delay_before_swap           0 // 445 [] http://www.opengl.org/registry/specs/NV/glx_delay_before_swap.txt
//#define GLX_MESA_query_renderer            0 // 446 [] http://www.opengl.org/registry/specs/MESA/glx_query_renderer.txt
//#define GL_NV_shader_thread_group          0 // 447 [] http://www.opengl.org/registry/specs/NV/shader_thread_group.txt
//#define GL_NV_shader_thread_shuffle        0 // 448 [] http://www.opengl.org/registry/specs/NV/shader_thread_shuffle.txt
//#define GL_EXT_shader_image_load_formatted 0 // 449 [] http://www.opengl.org/registry/specs/EXT/shader_image_load_formatted.txt
//#define GL_AMD_transform_feedback4         0 // 450 [] http://www.opengl.org/registry/specs/AMD/transform_feedback4.txt
//#define GL_AMD_gpu_shader_int64            0 // 451 [] http://www.opengl.org/registry/specs/AMD/gpu_shader_int64.txt
//#define GLX_EXT_stereo_tree                0 // 452 [] http://www.opengl.org/registry/specs/EXT/glx_stereo_tree.txt
//#define GL_AMD_gcn_shader                  0 // 453 [] http://www.opengl.org/registry/specs/AMD/gcn_shader.txt
//#define GL_NV_shader_atomic_int64          0 // 455 [] http://www.opengl.org/registry/specs/NV/shader_atomic_int64.txt
//#define GL_NV_bindless_multi_draw_indirect_count 0 // 456 [] http://www.opengl.org/registry/specs/NV/bindless_multi_draw_indirect_count.txt
//#define GLX_NV_copy_buffer                 0 // 457 [] http://www.opengl.org/registry/specs/NV/glx_copy_buffer.txt
//#define GL_KHR_blend_equation_advanced     0 // 458 [] http://www.opengl.org/registry/specs/KHR/blend_equation_advanced.txt
//#define GL_KHR_blend_equation_advanced_coherent 0 // ^458



















// nothing to bother from here on vvvvvvvvvvvvvvvvvvvvvvvvvvvvv

// if you disable specific stuff, some other stuff that is based on the disabled stuff will not work anymore ("or something" as Butthead would say)
#ifdef GL_VERSION_1_5
#if(GL_VERSION_1_5== 0)
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;
#endif
#endif

#ifdef GL_VERSION_2_0
#if(GL_VERSION_2_0== 0)
typedef char GLchar;
#endif
#endif

#ifdef GL_VERSION_3_2
#if(GL_VERSION_3_2== 0)
typedef struct __GLsync *GLsync;
typedef uint64_t GLuint64;
typedef int64_t GLint64;
#endif
#endif

#ifdef GL_ARB_bindless_texture
#if(GL_ARB_bindless_texture== 0)
typedef uint64_t GLuint64EXT;
#endif
#endif

#ifdef GL_ARB_vertex_buffer
#if(GL_ARB_vertex_buffer_object== 0)
typedef ptrdiff_t GLsizeiptrARB;
typedef ptrdiff_t GLintptrARB;
#endif
#endif

#ifdef WGL_ARB_pbuffer
#if(WGL_ARB_pbuffer== 2)
DECLARE_HANDLE(HPBUFFERARB);
#endif
#endif

#endif /// #ifndef OSI_GL_DISABLE - header end
#endif /// OSI_DISABLE_GL_DISABLE








