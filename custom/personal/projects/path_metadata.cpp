
#include <string.h>

#ifndef PPMETA_MAX_PATH
// NOTE(sammynilla): https://docs.microsoft.com/en-us/windows/win32/fileio/naming-a-file#maximum-path-length-limitation
#define PPMETA_MAX_PATH 260
#endif

// NOTE(sammynilla): For now we'll just use 20 as a base line.
#ifndef PPMETA_MAX_PROJECT_COUNT
#define PPMETA_MAX_PROJECT_COUNT 20
#endif

#ifndef PPMETA_FNAME
#define PPMETA_FNAME "projects.path"
#endif

struct Project_Path_Metadata
{
    char path[PPMETA_MAX_PROJECT_COUNT][PPMETA_MAX_PATH];
    i32 count;
};
global Project_Path_Metadata pp_meta = {};

internal void
init_project_path_metadata(void)
{
    FILE * file = nullptr;
    fopen_s(&file, PPMETA_FNAME, "a+");
    fclose(file);
    file = nullptr;
}

internal void
load_project_path_metadata(Application_Links * app)
{
    FILE * file = nullptr;
    char line[PPMETA_MAX_PATH];
    
    try_fopen:;
    const auto err = fopen_s(&file, PPMETA_FNAME, "r");
    
    if (err != 0) // NOTE(sammynilla): zero is successful for fopen_s
    {
        init_project_path_metadata();
        goto try_fopen;
    }
    
    while (fgets(line, PPMETA_MAX_PATH, file) != nullptr)
        ++pp_meta.count;
    
    rewind(file);
    
    i32 index = 0;
    while (fgets(line, PPMETA_MAX_PATH, file) != nullptr)
    {
        sscanf_s(line, "%s", line, PPMETA_MAX_PATH);
        strcpy(pp_meta.path[index], line);
        ++index;
        if (index >= PPMETA_MAX_PROJECT_COUNT)
            break;
    }
    
    fclose(file);
    file = nullptr;
}

internal void
save_project_path_metadata(void)
{
    FILE * file = nullptr;
    fopen_s(&file, PPMETA_FNAME, "w");
    
    for (i32 i = 0; i < pp_meta.count; ++i)
        fprintf(file, "%s\n", pp_meta.path[i]);
    
    fclose(file);
    file = nullptr;
}

internal void
push_project_path_metadata(const char * data)
{
    pp_meta.count =
        (pp_meta.count + 1 > PPMETA_MAX_PROJECT_COUNT) ?
        PPMETA_MAX_PROJECT_COUNT : ++pp_meta.count;
    
    // NOTE(sammynilla): Push everything down the stack
    for (i32 i = pp_meta.count; i > 0; --i)
        strcpy(pp_meta.path[i], pp_meta.path[i-1]);
    
    // NOTE(sammynilla): Overwrite the first index.
    strcpy(pp_meta.path[0], data);
}

// NOTE(sammynilla): In the event that the data is something that already exists,
// we should be popping it and placing it at the top of the stack.
internal void
move_project_path_at_index_to_front(int index)
{
    if (index == 0)
        return;
    
    char swap[PPMETA_MAX_PATH];
    // NOTE(sammynilla): store the value from indexed position
    strcpy(swap, pp_meta.path[index]);
    // NOTE(sammynilla): Push everything down the stack until we reach the original index.
    for (; index > 0; --index)
        strcpy(pp_meta.path[index], pp_meta.path[index-1]);
    // NOTE(sammynilla): Overwrite the first index.
    strcpy(pp_meta.path[0], swap);
}

/*
CUSTOM_COMMAND_SIG(debug_print_path_metadata)
CUSTOM_DOC("print out project path metadata")
{
    Scratch_Block scratch(app);
    String_Const_u8 dir = {};
    String_Const_u8 count = {};
    
    count = push_u8_stringf(scratch, "%i\n", pp_meta.count);
    print_message(app, count);
    
    for (i32 i = 0; i < pp_meta.count; ++i)
    {
        dir = push_u8_stringf(scratch, "%s\n", pp_meta.path[i]);
        print_message(app, dir);
    }
}
 */