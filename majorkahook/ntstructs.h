#pragma once
#include <Windows.h>

typedef struct _PEB_LDR_DATA {
    ULONG Length;                   
    BOOLEAN Initialized;            
    PVOID SsHandle;                 
    PLIST_ENTRY InLoadOrderModuleList; 
    PLIST_ENTRY InMemoryOrderModuleList; 
    PLIST_ENTRY InInitializationOrderModuleList; 
    PVOID EntryInProgress;          
    BOOLEAN ShutdownInProgress;     
    PVOID ShutdownThreadId;         
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING;


typedef struct _LDR_DATA_TABLE_ENTRY {
    PLIST_ENTRY InLoadOrderLinks;         
    PLIST_ENTRY InMemoryOrderLinks;       
    PLIST_ENTRY InInitializationOrderLinks; 
    PVOID DllBase;                       
    PVOID EntryPoint;                    
    ULONG SizeOfImage;                   
    UNICODE_STRING FullDllName;          
    UNICODE_STRING BaseDllName;          
    ULONG Flags;                         
    SHORT LoadCount;                     
    SHORT TlsIndex;                      
    PLIST_ENTRY HashLinks;                
    PVOID SectionPointer;                
    ULONG CheckSum;                      
    ULONG TimeDateStamp;                 
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
    ULONG MaximumLength;               
    ULONG Length;                      
    ULONG Flags;                       
    ULONG DebugFlags;                  
    HANDLE ConsoleHandle;              
    ULONG ConsoleFlags;                
    HANDLE StandardInput;              
    HANDLE StandardOutput;             
    HANDLE StandardError;              
    UNICODE_STRING CurrentDirectoryPath; 
    HANDLE CurrentDirectoryHandle;     
    UNICODE_STRING DllPath;            
    UNICODE_STRING ImagePathName;      
    UNICODE_STRING CommandLine;        
    PVOID Environment;                 
    ULONG StartingX;                   
    ULONG StartingY;                   
    ULONG CountX;                      
    ULONG CountY;                      
    ULONG CountCharsX;                 
    ULONG CountCharsY;                 
    ULONG FillAttribute;               
    ULONG WindowFlags;                 
    ULONG ShowWindowFlags;             
    UNICODE_STRING WindowTitle;        
    UNICODE_STRING DesktopInfo;        
    UNICODE_STRING ShellInfo;          
    UNICODE_STRING RuntimeData;        
    PVOID CurrentDirectories[32];      
    ULONG EnvironmentSize;             
    ULONG EnvironmentVersion;          
    PVOID PackageDependencyData;       
    ULONG ProcessGroupId;              
    ULONG LoaderThreads;               
    UNICODE_STRING RedirectionDllName; 
    UNICODE_STRING HeapPartitionName;  
    ULONG64 DefaultThreadpoolCpuSetMasks; 
    ULONG DefaultThreadpoolCpuSetMaskCount; 
} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;


typedef struct _PEB {
    BOOLEAN InheritedAddressSpace;   
    BOOLEAN ReadImageFileExecOptions; 
    BOOLEAN BeingDebugged;           
    BOOLEAN SpareBool;               
    HANDLE Mutant;                   
    PVOID ImageBaseAddress;          
    PPEB_LDR_DATA Ldr;               
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters; 
    PVOID SubSystemData;             
    PVOID ProcessHeap;               
    PVOID FastPebLock;               
    PVOID FastPebLockRoutine;        
    PVOID FastPebUnlockRoutine;      
    ULONG EnvironmentUpdateCount;    
    PVOID KernelCallbackTable;       
    ULONG SystemReserved[1];         
    ULONG AtlThunkSListPtr32;        
    PVOID FreeList;                  
    ULONG TlsExpansionCounter;       
    PVOID TlsBitmap;                 
    ULONG TlsBitmapBits[2];          
    PVOID ReadOnlySharedMemoryBase;  
    PVOID ReadOnlySharedMemoryHeap;  
    PVOID ReadOnlyStaticServerData;  
    PVOID AnsiCodePageData;          
    PVOID OemCodePageData;           
    PVOID UnicodeCaseTableData;      
    ULONG NumberOfProcessors;        
    ULONG NtGlobalFlag;              
    LARGE_INTEGER CriticalSectionTimeout; 
    ULONG HeapSegmentReserve;        
    ULONG HeapSegmentCommit;         
    ULONG HeapDeCommitTotalFreeThreshold; 
    ULONG HeapDeCommitFreeBlockThreshold; 
    ULONG NumberOfHeaps;             
    ULONG MaximumNumberOfHeaps;      
    PVOID* ProcessHeaps;             
    PVOID GdiSharedHandleTable;      
    PVOID ProcessStarterHelper;      
    ULONG GdiDCAttributeList;        
    PVOID LoaderLock;                
    ULONG OSMajorVersion;            
    ULONG OSMinorVersion;            
    ULONG OSBuildNumber;             
    ULONG OSPlatformId;              
    ULONG ImageSubsystem;            
    ULONG ImageSubsystemMajorVersion; 
    ULONG ImageSubsystemMinorVersion; 
    ULONG ActiveProcessAffinityMask; 
    ULONG GdiHandleBuffer[34];       
    PVOID PostProcessInitRoutine;    
    PVOID TlsExpansionBitmap;        
    ULONG TlsExpansionBitmapBits[32]; 
    ULONG SessionId;                 
} PEB, * PPEB;