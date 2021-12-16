i32 main(i32 args_count, char** args) {
    // TODO(geni): Rewrite this whole thing
    
    if (args_count == 2) {
        if (CStringCompareToS8(args[1], S8Lit("-h"))) {
            String8 base_name = S8PathToBaseName(S8FromCString(args[0]));
            printf("Usage: %.*s [-h] [-l] [filename (" DEFAULT_FILENAME ") [disabled patches]]]", S8VArg(base_name));
            return 0;
        } else if (CStringCompareToS8(args[1], S8Lit("-l"))) {
            puts("Available patches:");
            for (u32 i = 0; i < patches_count; ++i) {
                printf("Patch \"%.*s\"\n", S8VArg(patches[i]->name));
            }
            return 0;
        }
    }
    
    // NOTE(geni): User wants to disable specific patches
    if (args_count >= 3) {
        for (i32 i = 2; i < args_count; ++i) {
            for (u32 p = 0; p < patches_count; ++p) {
                if (patches[p] == NULL) {
                    continue;
                }
                
                if (S8CompareToCString(patches[p]->name, args[i])) {
                    printf("Disabling patch \"%.*s\"\n", S8VArg(patches[p]->name));
                    patches[p] = NULL;
                }
            }
        }
    }
    
    String8 input;
    if (args_count == 1) {
        input = S8Lit(DEFAULT_FILENAME);
        puts("No input filename passed, using \"" DEFAULT_FILENAME "\"");
    } else {
        input = S8FromCString(args[1]);
    }
    
#if BUILD_WIN32
    HANDLE file;
    {
        DWORD desired_access       = GENERIC_READ | GENERIC_WRITE;
        DWORD share_mode           = 0;
        DWORD creation_disposition = OPEN_EXISTING;
        DWORD flags_and_attributes = 0;
        
        if ((file = CreateFile(input.cs, desired_access, share_mode, 0, creation_disposition, flags_and_attributes, 0)) == INVALID_HANDLE_VALUE) {
            fprintf(stderr, "Failed to open file \"%.*s\" (error code %lu)\n", S8VArg(input), GetLastError());
            return 1;
        }
    }
    
    HANDLE file_mapping;
    {
        DWORD desired_access = PAGE_READWRITE;
        if ((file_mapping = CreateFileMappingA(file, 0, desired_access, 0, 0, 0)) == INVALID_HANDLE_VALUE) {
            fprintf(stderr, "Failed to create file mapping object (error code %lu)\n", GetLastError());
            return 1;
        }
    }
    
    u8* data;
    {
        DWORD desired_access = FILE_MAP_ALL_ACCESS;
        if ((data = MapViewOfFile(file_mapping, desired_access, 0, 0, 0)) == NULL) {
            fprintf(stderr, "Failed to create file view (error code %lu)\n", GetLastError());
            return 1;
        }
    }
#elif BUILD_LINUX
    i32 file;
    {
        i32 desired_access = O_RDWR;
        if ((file = open(input.cs, desired_access)) == -1) {
            fprintf(stderr, "Failed to open file \"%.*s\"\n", S8VArg(input));
            return 1;
        }
    }
    
    struct stat stats;
    if (stat(input.cs, &stats) == -1) {
        fputs("Failed to get file stats\n", stderr);
        return 1;
    }
    
    u8* data;
    {
        i32 desired_access = PROT_READ | PROT_WRITE;
        i32 visibility   = MAP_SHARED;
        if ((data = mmap(0, stats.st_size, desired_access, visibility, file, 0)) == MAP_FAILED) {
            fputs("Failed to map file\n", stderr);
            return 1;
        }
    }
#endif
    
    b32 failed_checks = 0;
    for (u32 i = 0; i < checks_count; ++i) {
        const Check* c = checks[i];
        
        if (memcmp(data + c->offset, PatternVArg(c->pattern)) != 0) {
            fprintf(stderr, "Check \"%.*s\" failed!\n", S8VArg(c->name));
            failed_checks = 1;
        }
    }
    if (failed_checks) {
        return 1;
    }
    
    for (u32 i = 0; i < patches_count; ++i) {
        const Patch* p = patches[i];
        
        // NOTE(geni): Patch is (hopefully) disabled
        if (p == NULL) {
            continue;
        }
        
        memcpy(data + p->offset, PatternVArg(p->pattern));
        printf("Patch \"%.*s\" done\n", S8VArg(p->name));
    }
}
