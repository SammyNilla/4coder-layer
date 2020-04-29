
#if !defined(FCODER_DEFAULT_BINDINGS_CPP)
#define FCODER_DEFAULT_BINDINGS_CPP

#include "4coder_default_include.cpp"
#include "generated/managed_id_metadata.cpp"
//https://www.includehelp.com/c-programs/define-a-macro-to-round-a-float-value-to-nearest-integer-in-c.aspx
#define Round(x) ((x)>=0 ? (long)((x)+0.5) : (long)((x)-0.5))
//|
#include "supported_languages.cpp"
#include "projects/path_metadata.cpp"
//|
// NOTE(sammynilla): Commands
#include "commands/build_panel.cpp"
#include "commands/execution.cpp"
#include "commands/file.cpp"
#include "commands/project.cpp"
//|
// NOTE(sammynilla): UI/Listers
#include "commands/ui/project.cpp"
//|
#include "hooks.cpp"
#include "mapping.cpp"
#include "renderer.cpp"

void
custom_layer_init(Application_Links * App)
{
    Thread_Context * ThreadContext = get_thread_context(App);
    
    // NOTE(allen): setup for default framework
    default_framework_init(App);
    
    // NOTE(allen): default hooks and command maps
    set_all_default_hooks(App);
    set_custom_hook(App, HookID_BufferViewerUpdate, 0);
    set_custom_hook(App, HookID_RenderCaller, sn_RenderCaller);
    set_custom_hook(App, HookID_BeginBuffer, sn_BeginBuffer);
    set_custom_hook(App, HookID_BufferRegion, sn_BufferRegion);
    
    mapping_init(ThreadContext, &framework_mapping);
    sn_SetInputMapping(&framework_mapping, mapid_global, mapid_file, mapid_code);
}

#endif