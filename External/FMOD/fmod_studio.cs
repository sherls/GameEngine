//#define DISABLE_LOW_LEVEL

/* ========================================================================================== */
/* FMOD System - C# Wrapper . Copyright (c), Firelight Technologies Pty, Ltd. 2004-2014.       */
/*                                                                                            */
/*                                                                                            */
/* ========================================================================================== */

using System;
using System.Text;
using System.Runtime.InteropServices;

namespace FMOD
{
namespace Studio
{
    public class STUDIO_VERSION
    {
        public const string dll    = "fmodstudio.dll";
    }
        
    public enum LOADING_MODE
    {
        BEGIN_NOW,
        PROHIBITED
    }
        
    public enum STOP_MODE
    {
        ALLOWFADEOUT,
        IMMEDIATE
    }

    public enum LOADING_STATE
    {
        UNLOADING,
        UNLOADED,
        LOADING,
        LOADED
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct _3D_ATTRIBUTES
    {
        public VECTOR position;
        public VECTOR velocity;
        public VECTOR forward;
        public VECTOR up;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    public struct PROGRAMMER_SOUND_PROPERTIES
    {
        public string name;
        public IntPtr eventInstance;
        public IntPtr sound;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct ADVANCEDSETTINGS
    {
        public int cbSize;               /* [w]   Size of this structure.  NOTE: For C# wrapper, users can leave this at 0. ! */
        public int commandQueueSize;     /* [r/w] Optional. Specify 0 to ignore. Specify the command queue size for studio async processing.  Default 4096 (4kb) */
        public int handleInitialSize;    /* [r/w] Optional. Specify 0 to ignore. Specify the initial size to allocate for handles.  Memory for handles will grow as needed in pages. */
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct CPU_USAGE
    {
        public float dspUsage;            /* Returns the % CPU time taken by DSP processing on the low level mixer thread. */
        public float streamUsage;         /* Returns the % CPU time taken by stream processing on the low level stream thread. */
        public float geometryUsage;       /* Returns the % CPU time taken by geometry processing on the low level geometry thread. */
        public float updateUsage;         /* Returns the % CPU time taken by low level update, called as part of the studio update. */
        public float studioUsage;         /* Returns the % CPU time taken by studio update, called from the studio thread. Does not include low level update time. */
    }
    public enum PARAMETER_TYPE
    {
        GAME_CONTROLLED,                  /* Controlled via the API using Studio::ParameterInstance::setValue. */
        AUTOMATIC_DISTANCE,               /* Distance between the event and the listener. */
        AUTOMATIC_EVENT_CONE_ANGLE,       /* Angle between the event's forward vector and the vector pointing from the event to the listener (0 to 180 degrees). */
        AUTOMATIC_EVENT_ORIENTATION,      /* Horizontal angle between the event's forward vector and listener's forward vector (-180 to 180 degrees). */
        AUTOMATIC_DIRECTION,              /* Horizontal angle between the listener's forward vector and the vector pointing from the listener to the event (-180 to 180 degrees). */
        AUTOMATIC_ELEVATION,              /* Angle between the listener's XZ plane and the vector pointing from the listener to the event (-90 to 90 degrees). */
        AUTOMATIC_LISTENER_ORIENTATION,   /* Horizontal angle between the listener's forward vector and the global positive Z axis (-180 to 180 degrees). */
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct PARAMETER_DESCRIPTION
    {
        public string name;                                /* Name of the parameter. */
        public float minimum;                              /* Minimum parameter value. */
        public float maximum;                              /* Maximum parameter value. */
        public PARAMETER_TYPE type;                        /* Type of the parameter */
    }

    #region wrapperinternal
    // The above structure has an issue with getting a const char* back from game code so we use this special marshalling struct instead
    [StructLayout(LayoutKind.Sequential)]
    struct PARAMETER_DESCRIPTION_INTERNAL
    {
        public IntPtr name;                                /* Name of the parameter. */
        public float minimum;                              /* Minimum parameter value. */
        public float maximum;                              /* Maximum parameter value. */
        public PARAMETER_TYPE type;                        /* Type of the parameter */

        // Helper functions
        static string stringFromNativeUtf8(IntPtr nativeUtf8) 
        {
            // There is no one line marshal IntPtr->string for UTF8
            int len = 0;
            while (Marshal.ReadByte(nativeUtf8, len) != 0) ++len;
            if (len == 0) return string.Empty;
            byte[] buffer = new byte[len];
            Marshal.Copy(nativeUtf8, buffer, 0, buffer.Length);
            return Encoding.UTF8.GetString(buffer);
        }
        // Helper functions
        public void assign(out PARAMETER_DESCRIPTION publicDesc)
        {
            publicDesc.name = stringFromNativeUtf8(name);
            publicDesc.minimum = minimum;
            publicDesc.maximum = maximum;
            publicDesc.type = type;
        }
    }
    // This is only need for loading memory and given our C# wrapper LOAD_MEMORY_POINT isn't feasible anyway
    enum LOAD_MEMORY_MODE
    {
        LOAD_MEMORY,
        LOAD_MEMORY_POINT
    }

    #endregion

    [Flags]
    public enum INITFLAGS
    {
        NORMAL                  = 0x00000000,   /* Initialize normally. */
        LIVEUPDATE              = 0x00000001,   /* Enable live update. */
        ALLOW_MISSING_PLUGINS   = 0x00000002,   /* Load banks even if they reference plugins that have not been loaded. */
        SYNCHRONOUS_UPDATE      = 0x00000004,   /* Disable asynchronous processing and perform all processing on the calling thread instead. */
    }

    [Flags]
    public enum LOAD_BANK_FLAGS
    {
        NORMAL      = 0x00000000,   /* Standard behaviour. */
        NONBLOCKING = 0x00000001,   /* Bank loading occurs asynchronously rather than occurring immediately. */
    }

    [Flags]
    public enum RECORD_COMMANDS_FLAGS
    {
        NORMAL      = 0x00000000,   /* Standard behaviour. */
        FILEFLUSH   = 0x00000001,   /* Call file flush on every command. */
    }

    public enum PLAYBACK_STATE
    {
        PLAYING,
        IDLE,
        SUSTAINING,
        STOPPED
    }

    public enum EVENT_CALLBACK_TYPE
    {
        STARTED,                    /* Called when an instance starts. Parameters = FMOD_STUDIO_EVENTINSTANCE, which can be cast to Studio::EventInstance* type. */
        STOPPED,                    /* Called when an instance stops. Parameters = FMOD_STUDIO_EVENTINSTANCE, which can be cast to Studio::EventInstance* type. */
        IDLE,                       /* Called when an instance enters the idle state. Parameters = FMOD_STUDIO_EVENTINSTANCE, which can be cast to Studio::EventInstance* type. */
        CREATE_PROGRAMMER_SOUND,    /* Called when a programmer sound needs to be created in order to play a programmer instrument. Parameters = FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES. */
        DESTROY_PROGRAMMER_SOUND    /* Called when a programmer sound needs to be destroyed. Parameters = FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES. */
    }

    public delegate RESULT EVENT_CALLBACK(EVENT_CALLBACK_TYPE type, IntPtr parameters);
    
    public class Factory
    {
        public static RESULT System_Create(out System studiosystem)
        {
            RESULT      result           = RESULT.OK;
            IntPtr      rawPtr  = new IntPtr();
            studiosystem                 = null;

            result = FMOD_Studio_System_Create(out rawPtr, VERSION.number);
            if (result != RESULT.OK)
            {
                return result;
            }

            studiosystem = new System(rawPtr);

            return result;
        }

        #region importfunctions

        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_Create                      (out IntPtr studiosystem, uint headerversion);
            
        #endregion
    }
    
    public class Util
    {
        public static RESULT ParseID(string idString, out GUID id)
        {
            return FMOD_Studio_ParseID(Encoding.UTF8.GetBytes(idString + Char.MinValue), out id);
        }

        #region importfunctions

        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_ParseID                      (byte[] idString, out GUID id);

        #endregion
    }
    
    public class HandleBase
    {
        public HandleBase(IntPtr newPtr)
        {
            rawPtr = newPtr;
        }

        public bool isValid()
        {
            return rawPtr != IntPtr.Zero;
        }

        public IntPtr getRaw()
        {
            return rawPtr;
        }

        protected IntPtr rawPtr;

        #region equality

        public override bool Equals(Object obj)
        {
            return Equals(obj as HandleBase);
        }
        public bool Equals(HandleBase p)
        {
            // Equals if p not null and handle is the same
            return ((object)p != null && rawPtr == p.rawPtr);
        }
        public override int GetHashCode()
        {
            return rawPtr.ToInt32();
        }
        public static bool operator ==(HandleBase a, HandleBase b)
        {
            // If both are null, or both are same instance, return true.
            if (Object.ReferenceEquals(a, b))
            {
                return true;
            }
            // If one is null, but not both, return false.
            if (((object)a == null) || ((object)b == null))
            {
                return false;
            }
            // Return true if the handle matches
            return (a.rawPtr == b.rawPtr);
        }
        public static bool operator !=(HandleBase a, HandleBase b)
        {
            return !(a == b);
        }
        #endregion

    }

    public class System : HandleBase
    {
        // Initialization / system functions.
        public RESULT setAdvancedSettings(ADVANCEDSETTINGS settings)
        {
            settings.cbSize = Marshal.SizeOf(new ADVANCEDSETTINGS());
            return FMOD_Studio_System_SetAdvancedSettings(rawPtr, ref settings);
        }
        public RESULT getAdvancedSettings(out ADVANCEDSETTINGS settings)
        {
            settings.cbSize = Marshal.SizeOf(new ADVANCEDSETTINGS());
            return FMOD_Studio_System_GetAdvancedSettings(rawPtr, out settings);
        }
        public RESULT init(int maxchannels, INITFLAGS studioFlags, FMOD.INITFLAGS flags, IntPtr extradriverdata)
        {
            return FMOD_Studio_System_Initialize(rawPtr, maxchannels, studioFlags, flags, extradriverdata);
        }
        public RESULT release()
        {
            return FMOD_Studio_System_Release(rawPtr);
        }
        public RESULT update()
        {
            return FMOD_Studio_System_Update(rawPtr);
        }
#if !DISABLE_LOW_LEVEL
        public RESULT getLowLevelSystem(out FMOD.System system)
        {
            system = null;

            IntPtr systemraw = new IntPtr();
            RESULT result = FMOD_Studio_System_GetLowLevelSystem(rawPtr, out systemraw);
            if (result != RESULT.OK)
            {
                return result;
            }

            system = new FMOD.System();
            system.setRaw(systemraw);

            return result;
        }
#endif
        public RESULT getEvent(GUID guid, LOADING_MODE mode, out EventDescription _event)
        {
            _event = null;

            IntPtr eventraw = new IntPtr();
            RESULT result = FMOD_Studio_System_GetEvent(rawPtr, ref guid, mode, out eventraw);
            if (result != RESULT.OK)
            {
                return result;
            }

            _event = new EventDescription(eventraw);
            return result;
        }
        public RESULT getMixerStrip(GUID guid, LOADING_MODE mode, out MixerStrip strip)
        {
            strip = null;

            IntPtr newPtr = new IntPtr();
            RESULT result = FMOD_Studio_System_GetMixerStrip(rawPtr, ref guid, mode, out newPtr);
            if (result != RESULT.OK)
            {
                return result;
            }

            strip = new MixerStrip(newPtr);
            return result;
        }

        public RESULT getBank(GUID guid, out Bank bank)
        {
            bank = null;

            IntPtr newPtr = new IntPtr();
            RESULT result = FMOD_Studio_System_GetBank(rawPtr, ref guid, out newPtr);
            if (result != RESULT.OK)
            {
                return result;
            }

            bank = new Bank(newPtr);
            return result;
        }
        public RESULT lookupID(string path, out GUID guid)
        {
            return FMOD_Studio_System_LookupID(rawPtr, Encoding.UTF8.GetBytes(path + Char.MinValue), out guid);
        }
        public RESULT lookupPath(GUID guid, out string path)
        {
            path = null;

            byte[] buffer = new byte[256];
            int retrieved = 0;
            RESULT result = FMOD_Studio_System_LookupPath(rawPtr, ref guid, buffer, buffer.Length, out retrieved);

            if (result == RESULT.ERR_TRUNCATED)
            {
                buffer = new byte[retrieved];
                result = FMOD_Studio_System_LookupPath(rawPtr, ref guid, buffer, buffer.Length, out retrieved);
            }

            if (result == RESULT.OK)
            {
                path = Encoding.UTF8.GetString(buffer, 0, retrieved - 1);
            }

            return result;
        }
        public RESULT getListenerAttributes(out _3D_ATTRIBUTES attributes)
        {
            return FMOD_Studio_System_GetListenerAttributes(rawPtr, out attributes);
        }
        public RESULT setListenerAttributes(_3D_ATTRIBUTES attributes)
        {
            return FMOD_Studio_System_SetListenerAttributes(rawPtr, ref attributes);
        }
        public RESULT loadBankFile(string name, LOAD_BANK_FLAGS flags, out Bank bank)
        {
            bank = null;

            IntPtr newPtr = new IntPtr();
            RESULT result = FMOD_Studio_System_LoadBankFile(rawPtr, Encoding.UTF8.GetBytes(name + Char.MinValue), flags, out newPtr);
            if (result != RESULT.OK)
            {
                return result;
            }

            bank = new Bank(newPtr);
            return result;
        }
        
        public RESULT loadBankMemory(byte[] buffer, LOAD_BANK_FLAGS flags, out Bank bank)
        {
            bank = null;

            IntPtr newPtr = new IntPtr();
            RESULT result = FMOD_Studio_System_LoadBankMemory(rawPtr, buffer, buffer.Length, LOAD_MEMORY_MODE.LOAD_MEMORY, flags, out newPtr);
            if (result != RESULT.OK)
            {
                return result;
            }

            bank = new Bank(newPtr);
            return result;
        }

        public RESULT unloadAll()
        {
            return FMOD_Studio_System_UnloadAll(rawPtr);
        }

        public RESULT flushCommands()
        {
            return FMOD_Studio_System_FlushCommands(rawPtr);
        }
        public RESULT startRecordCommands(string path, RECORD_COMMANDS_FLAGS flags)
        {
            return FMOD_Studio_System_StartRecordCommands(rawPtr, Encoding.UTF8.GetBytes(path + Char.MinValue), flags);
        }
        public RESULT stopRecordCommands()
        {
            return FMOD_Studio_System_StopRecordCommands(rawPtr);
        }
        public RESULT playbackCommands(string path)
        {
            return FMOD_Studio_System_PlaybackCommands(rawPtr, Encoding.UTF8.GetBytes(path + Char.MinValue));
        }
        public RESULT getBankCount(out int count)
        {
            return FMOD_Studio_System_GetBankCount(rawPtr, out count);
        }
        public RESULT getBankList(out Bank[] array)
        {
            array = null;

            RESULT result;
            int capacity;
            result = FMOD_Studio_System_GetBankCount(rawPtr, out capacity);
            if (result != RESULT.OK)
            {
                return result;
            }
            if (capacity == 0)
            {
                array = new Bank[0];
                return result;
            }

            IntPtr[] rawArray = new IntPtr[capacity];
            int actualCount;
            result = FMOD_Studio_System_GetBankList(rawPtr, rawArray, capacity, out actualCount);
            if (result != RESULT.OK)
            {
                return result;
            }
            if (actualCount > capacity) // More items added since we queried just now?
            {
                actualCount = capacity;
            }
            array = new Bank[actualCount];
            for (int i=0; i<actualCount; ++i)
            {
                array[i] = new Bank(rawArray[i]);
            }
            return RESULT.OK;
        }
        
        public RESULT getCPUUsage(out CPU_USAGE usage)
        {
            return FMOD_Studio_System_GetCPUUsage(rawPtr, out usage);
        }

        #region importfunctions
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_SetAdvancedSettings     (IntPtr studiosystem, ref ADVANCEDSETTINGS settings);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_GetAdvancedSettings     (IntPtr studiosystem, out ADVANCEDSETTINGS settings);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_Initialize              (IntPtr studiosystem, int maxchannels, INITFLAGS studioFlags, FMOD.INITFLAGS flags, IntPtr extradriverdata);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_Release                 (IntPtr studiosystem);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_Update                  (IntPtr studiosystem);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_GetLowLevelSystem       (IntPtr studiosystem, out IntPtr system);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_GetEvent                (IntPtr studiosystem, ref GUID guid, LOADING_MODE mode, out IntPtr description);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_GetMixerStrip           (IntPtr studiosystem, ref GUID guid, LOADING_MODE mode, out IntPtr mixerStrip);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_GetBank                 (IntPtr studiosystem, ref GUID guid, out IntPtr bank);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_LookupID                (IntPtr studiosystem, byte[] path, out GUID guid);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_LookupPath              (IntPtr studiosystem, ref GUID guid, [Out] byte[] path, int size, out int retrieved);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_GetListenerAttributes   (IntPtr studiosystem, out _3D_ATTRIBUTES attributes);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_SetListenerAttributes   (IntPtr studiosystem, ref _3D_ATTRIBUTES attributes);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_LoadBankFile            (IntPtr studiosystem, byte[] filename, LOAD_BANK_FLAGS flags, out IntPtr bank);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_LoadBankMemory          (IntPtr studiosystem, byte[] buffer, int length, LOAD_MEMORY_MODE mode, LOAD_BANK_FLAGS flags, out IntPtr bank);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_UnloadAll               (IntPtr studiosystem);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_FlushCommands           (IntPtr studiosystem);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_StartRecordCommands     (IntPtr studiosystem, byte[] path, RECORD_COMMANDS_FLAGS flags);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_StopRecordCommands      (IntPtr studiosystem);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_PlaybackCommands        (IntPtr studiosystem, byte[] path);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_GetBankCount            (IntPtr studiosystem, out int count);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_GetBankList             (IntPtr studiosystem, IntPtr[] array, int capacity, out int count);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_System_GetCPUUsage             (IntPtr studiosystem, out CPU_USAGE usage);

        #endregion
        #region wrapperinternal

        public System(IntPtr raw)
        : base(raw)
        {
        }

        #endregion
    }
    
    public class EventDescription : HandleBase
    {
        public RESULT getID(out GUID id)
        {
            return FMOD_Studio_EventDescription_GetID(rawPtr, out id);
        }
        public RESULT getPath(out string path)
        {
            path = null;

            byte[] buffer = new byte[256];
            int retrieved = 0;
            RESULT result = FMOD_Studio_EventDescription_GetPath(rawPtr, buffer, buffer.Length, out retrieved);

            if (result == RESULT.ERR_TRUNCATED)
            {
                buffer = new byte[retrieved];
                result = FMOD_Studio_EventDescription_GetPath(rawPtr, buffer, buffer.Length, out retrieved);
            }

            if (result == RESULT.OK)
            {
                path = Encoding.UTF8.GetString(buffer, 0, retrieved - 1);
            }

            return result;
        }
        public RESULT getParameterCount(out int count)
        {
            return FMOD_Studio_EventDescription_GetParameterCount(rawPtr, out count);
        }
        public RESULT getParameterByIndex(int index, out PARAMETER_DESCRIPTION parameter)
        {
            parameter = new PARAMETER_DESCRIPTION();

            PARAMETER_DESCRIPTION_INTERNAL paramInternal;
            RESULT result = FMOD_Studio_EventDescription_GetParameterByIndex(rawPtr, index, out paramInternal);
            if (result != RESULT.OK)
            {
                return result;
            }
            paramInternal.assign(out parameter);
            return result;
        }
        public RESULT getParameter(string name, out PARAMETER_DESCRIPTION parameter)
        {
            parameter = new PARAMETER_DESCRIPTION();

            PARAMETER_DESCRIPTION_INTERNAL paramInternal;
            RESULT result = FMOD_Studio_EventDescription_GetParameter(rawPtr, Encoding.UTF8.GetBytes(name + Char.MinValue), out paramInternal);
            if (result != RESULT.OK)
            {
                return result;
            }
            paramInternal.assign(out parameter);
            return result;
        }
        public RESULT getLength(out int length)
        {
            return FMOD_Studio_EventDescription_GetLength(rawPtr, out length);
        }
        public RESULT getMinimumDistance(out float distance)
        {
            return FMOD_Studio_EventDescription_GetMinimumDistance(rawPtr, out distance);
        }
        public RESULT getMaximumDistance(out float distance)
        {
            return FMOD_Studio_EventDescription_GetMaximumDistance(rawPtr, out distance);
        }
        public RESULT isOneshot(out bool oneshot)
        {
            return FMOD_Studio_EventDescription_IsOneshot(rawPtr, out oneshot);
        }
        public RESULT isStream(out bool isStream)
        {
            return FMOD_Studio_EventDescription_IsStream(rawPtr, out isStream);
        }
        public RESULT is3D(out bool is3D)
        {
            return FMOD_Studio_EventDescription_Is3D(rawPtr, out is3D);
        }

        public RESULT createInstance(out EventInstance instance)
        {
            instance = null;

            IntPtr newPtr = new IntPtr();
            RESULT result = FMOD_Studio_EventDescription_CreateInstance(rawPtr, out newPtr);
            if (result != RESULT.OK)
            {
                return result;
            }
            instance = new EventInstance(newPtr);
            return result;
        }

        public RESULT getInstanceCount(out int count)
        {
            return FMOD_Studio_EventDescription_GetInstanceCount(rawPtr, out count);
        }
        public RESULT getInstanceList(out EventInstance[] array)
        {
            array = null;

            RESULT result;
            int capacity;
            result = FMOD_Studio_EventDescription_GetInstanceCount(rawPtr, out capacity);
            if (result != RESULT.OK)
            {
                return result;
            }
            if (capacity == 0)
            {
                array = new EventInstance[0];
                return result;
            }

            IntPtr[] rawArray = new IntPtr[capacity];
            int actualCount;
            result = FMOD_Studio_EventDescription_GetInstanceList(rawPtr, rawArray, capacity, out actualCount);
            if (result != RESULT.OK)
            {
                return result;
            }
            if (actualCount > capacity) // More items added since we queried just now?
            {
                actualCount = capacity;
            }
            array = new EventInstance[actualCount];
            for (int i=0; i<actualCount; ++i)
            {
                array[i] = new EventInstance(rawArray[i]);
            }
            return RESULT.OK;
        }

        public RESULT releaseAllInstances()
        {
            return FMOD_Studio_EventDescription_ReleaseAllInstances(rawPtr);
        }
        public RESULT setCallback(EVENT_CALLBACK callback)
        {
            return FMOD_Studio_EventDescription_SetCallback(rawPtr, callback);
        }

        #region importfunctions
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_GetID(IntPtr eventdescription, out GUID id);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_GetPath(IntPtr eventdescription, [Out] byte[] path, int size, out int retrieved);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_GetParameterCount(IntPtr eventdescription, out int count);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_GetParameterByIndex(IntPtr eventdescription, int index, out PARAMETER_DESCRIPTION_INTERNAL parameter);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_GetParameter(IntPtr eventdescription, byte[] name, out PARAMETER_DESCRIPTION_INTERNAL parameter);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_GetUserPropertyCount(IntPtr eventdescription, out int count);
        //[DllImport (STUDIO_VERSION.dll)]
        //private static extern RESULT FMOD_Studio_EventDescription_GetUserPropertyByIndex(IntPtr eventdescription, int index, out USER_PROPERTY property);
        //[DllImport (STUDIO_VERSION.dll)]
        //private static extern RESULT FMOD_Studio_EventDescription_GetUserProperty(IntPtr eventdescription, byte[] name, out USER_PROPERTY property);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_GetLength(IntPtr eventdescription, out int length);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_GetMinimumDistance(IntPtr eventdescription, out float distance);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_GetMaximumDistance(IntPtr eventdescription, out float distance);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_IsOneshot(IntPtr eventdescription, out bool oneshot);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_IsStream(IntPtr eventdescription, out bool isStream);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_Is3D(IntPtr eventdescription, out bool is3D);        
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_CreateInstance(IntPtr eventdescription, out IntPtr instance);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_GetInstanceCount(IntPtr eventdescription, out int count);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_GetInstanceList(IntPtr eventdescription, IntPtr[] array, int capacity, out int count);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_ReleaseAllInstances(IntPtr eventdescription);
        [DllImport (STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventDescription_SetCallback(IntPtr eventdescription, EVENT_CALLBACK callback);
        //[DllImport (STUDIO_VERSION.dll)]
        //private static extern RESULT FMOD_Studio_EventDescription_GetUserData(IntPtr eventdescription, void **userData);
        //[DllImport (STUDIO_VERSION.dll)]
        //private static extern RESULT FMOD_Studio_EventDescription_SetUserData(IntPtr eventdescription, void *userData);   
        #endregion
        #region wrapperinternal

        public EventDescription(IntPtr raw)
        : base(raw)
        {
        }

        #endregion
    }

    public class EventInstance : HandleBase
    {
        public RESULT getDescription(out EventDescription description)
        {
            description = null;

            IntPtr newPtr;
            RESULT result = FMOD_Studio_EventInstance_GetDescription(rawPtr, out newPtr);
            if (result != RESULT.OK)
            {
                return result;
            }
            description = new EventDescription(newPtr);
            return result;
        }

        public RESULT getVolume(out float volume)
        {
            return FMOD_Studio_EventInstance_GetVolume(rawPtr, out volume);
        }
        public RESULT setVolume(float volume)
        {
            return FMOD_Studio_EventInstance_SetVolume(rawPtr, volume);
        }
        public RESULT getPitch(out float pitch)
        {
            return FMOD_Studio_EventInstance_GetPitch(rawPtr, out pitch);
        }
        public RESULT setPitch(float pitch)
        {
            return FMOD_Studio_EventInstance_SetPitch(rawPtr, pitch);
        }
        public RESULT set3DAttributes               (_3D_ATTRIBUTES attributes)
        {
            return FMOD_Studio_EventInstance_Set3DAttributes(rawPtr, ref attributes);
        }
        public RESULT getPaused(out bool paused)
        {
            int p = 0;
            RESULT result = FMOD_Studio_EventInstance_GetPaused(rawPtr, out p);
            paused = (p != 0);
            return result;
        }
        public RESULT setPaused(bool paused)
        {
            return FMOD_Studio_EventInstance_SetPaused(rawPtr, (paused ? 1 : 0));
        }
        public RESULT start()
        {
            return FMOD_Studio_EventInstance_Start(rawPtr);
        }
        public RESULT stop(STOP_MODE mode)
        {
            return FMOD_Studio_EventInstance_Stop(rawPtr, mode);
        }
        public RESULT getTimelinePosition(out int position)
        {
            return FMOD_Studio_EventInstance_GetTimelinePosition(rawPtr, out position);
        }
        public RESULT setTimelinePosition(int position)
        {
            return FMOD_Studio_EventInstance_SetTimelinePosition(rawPtr, position);
        }
        public RESULT getPlaybackState(out PLAYBACK_STATE state)
        {
            return FMOD_Studio_EventInstance_GetPlaybackState(rawPtr, out state);
        }
#if !DISABLE_LOW_LEVEL
        public RESULT getChannelGroup(out FMOD.ChannelGroup group)
        {
            group = null;

            IntPtr groupraw = new IntPtr();
            RESULT result = FMOD_Studio_EventInstance_GetChannelGroup(rawPtr, out groupraw);
            if (result != RESULT.OK)
            {
                return result;
            }

            group = new FMOD.ChannelGroup();
            group.setRaw(groupraw);

            return result;
        }
#endif
        public RESULT release()
        {
            return FMOD_Studio_EventInstance_Release(rawPtr);
        }
        public RESULT getParameter(string name, out ParameterInstance instance)
        {
            instance = null;

            IntPtr newPtr = new IntPtr();
            RESULT result = FMOD_Studio_EventInstance_GetParameter(rawPtr, Encoding.UTF8.GetBytes(name + Char.MinValue), out newPtr);
            if (result != RESULT.OK)
            {
                return result;
            }
            instance = new ParameterInstance(newPtr);

            return result;
        }
        public RESULT getParameterCount(out int count)
        {
            return FMOD_Studio_EventInstance_GetParameterCount(rawPtr, out count);
        }
        public RESULT getParameterByIndex(int index, out ParameterInstance instance)
        {
            instance = null;

            IntPtr newPtr = new IntPtr();
            RESULT result = FMOD_Studio_EventInstance_GetParameterByIndex(rawPtr, index, out newPtr);
            if (result != RESULT.OK)
            {
                return result;
            }
            instance = new ParameterInstance(newPtr);

            return result;
        }
        public RESULT setParameterValue(string name, float value)
        {
            return FMOD_Studio_EventInstance_SetParameterValue(rawPtr, Encoding.UTF8.GetBytes(name + Char.MinValue), value);
        }
        public RESULT setParameterValueByIndex(int index, float value)
        {
            return FMOD_Studio_EventInstance_SetParameterValueByIndex(rawPtr, index, value);
        }

        public RESULT getCue(string name, out CueInstance instance)
        {
            instance = null;

            IntPtr newPtr = new IntPtr();
            RESULT result = FMOD_Studio_EventInstance_GetCue(rawPtr, Encoding.UTF8.GetBytes(name + Char.MinValue), out newPtr);
            if (result != RESULT.OK)
            {
                return result;
            }
            instance = new CueInstance(newPtr);

            return result;
        }

        public RESULT getCueByIndex(int index, out CueInstance instance)
        {
            instance = null;

            IntPtr newPtr = new IntPtr();
            RESULT result = FMOD_Studio_EventInstance_GetCueByIndex(rawPtr, index, out newPtr);
            if (result != RESULT.OK)
            {
                return result;
            }
            instance = new CueInstance(newPtr);

            return result;
        }
        public RESULT getCueCount(out int count)
        {
            return FMOD_Studio_EventInstance_GetCueCount(rawPtr, out count);
        }

        public RESULT createSubEvent(string name, out EventInstance instance)
        {
            instance = null;

            IntPtr newPtr = new IntPtr();
            RESULT result = FMOD_Studio_EventInstance_CreateSubEvent(rawPtr, Encoding.UTF8.GetBytes(name + Char.MinValue), out newPtr);
            if (result != RESULT.OK)
            {
                return result;
            }
            instance = new EventInstance(newPtr);

            return result;
        }
        public RESULT getLoadingState(out LOADING_STATE state)
        {
            return FMOD_Studio_EventInstance_GetLoadingState(rawPtr, out state);
        }
        public RESULT setCallback(EVENT_CALLBACK callback)
        {
            return FMOD_Studio_EventInstance_SetCallback(rawPtr, callback);
        }

        #region importfunctions

        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_GetDescription       (IntPtr _event, out IntPtr description);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_GetVolume            (IntPtr _event, out float volume);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_SetVolume            (IntPtr _event, float volume);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_GetPitch             (IntPtr _event, out float pitch);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_SetPitch             (IntPtr _event, float pitch);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_Get3DAttributes      (IntPtr _event, out _3D_ATTRIBUTES attributes);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_Set3DAttributes      (IntPtr _event, ref _3D_ATTRIBUTES attributes);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_GetPaused            (IntPtr _event, out int paused);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_SetPaused            (IntPtr _event, int paused);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_Start                (IntPtr _event);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_Stop                 (IntPtr _event, STOP_MODE mode);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_GetTimelinePosition  (IntPtr _event, out int position);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_SetTimelinePosition  (IntPtr _event, int position);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_GetPlaybackState     (IntPtr _event, out PLAYBACK_STATE state);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_GetChannelGroup      (IntPtr _event, out IntPtr group);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_Release              (IntPtr _event);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_IsVirtual            (IntPtr _event, out bool virtualState);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_GetParameter         (IntPtr _event, byte[] name, out IntPtr parameter);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_GetParameterByIndex  (IntPtr _event, int index, out IntPtr parameter);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_GetParameterCount    (IntPtr _event, out int count);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_SetParameterValue    (IntPtr _event, byte[] name, float value);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_SetParameterValueByIndex (IntPtr _event, int index, float value);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_GetCue               (IntPtr _event, byte[] name, out IntPtr cue);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_GetCueByIndex        (IntPtr _event, int index, out IntPtr cue);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_GetCueCount          (IntPtr _event, out int count);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_CreateSubEvent       (IntPtr _event, byte[] name, out IntPtr _instance);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_GetLoadingState      (IntPtr _event, out LOADING_STATE state);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_EventInstance_SetCallback          (IntPtr _event, EVENT_CALLBACK callback);   
        
        #endregion
        #region wrapperinternal

        public EventInstance(IntPtr raw)
        : base(raw)
        {
        }

        #endregion
    }

    public class CueInstance : HandleBase
    {
        public RESULT trigger()
        {
            return FMOD_Studio_CueInstance_Trigger(rawPtr);
        }

        #region importfunctions

        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_CueInstance_Trigger(IntPtr cue);

        #endregion
        #region wrapperinternal

        public CueInstance(IntPtr raw)
        : base(raw)
        {
        }

        #endregion
    }

    public class ParameterInstance : HandleBase
    {
        public RESULT getDescription(out PARAMETER_DESCRIPTION description)
        {
            description = new PARAMETER_DESCRIPTION();

            PARAMETER_DESCRIPTION_INTERNAL paramInternal;
            RESULT result = FMOD_Studio_ParameterInstance_GetDescription(rawPtr, out paramInternal);
            if (result != RESULT.OK)
            {
                return result;
            }
            paramInternal.assign(out description);
            return result;
        }

        public RESULT getValue(out float value)
        {
            return FMOD_Studio_ParameterInstance_GetValue(rawPtr, out value);
        }
        public RESULT setValue(float value)
        {
            return FMOD_Studio_ParameterInstance_SetValue(rawPtr, value);
        }

        #region importfunctions

        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_ParameterInstance_GetDescription(IntPtr parameter, out PARAMETER_DESCRIPTION_INTERNAL description);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_ParameterInstance_GetValue(IntPtr parameter, out float value);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_ParameterInstance_SetValue(IntPtr parameter, float value);

        #endregion
        #region wrapperinternal

        public ParameterInstance(IntPtr raw)
        : base(raw)
        {
        }

        #endregion
    }

    public class MixerStrip : HandleBase
    {
        public RESULT getID(out GUID id)
        {
            return FMOD_Studio_MixerStrip_GetID(rawPtr, out id);
        }
        public RESULT getPath(out string path)
        {
            path = null;

            byte[] buffer = new byte[256];
            int retrieved = 0;
            RESULT result = FMOD_Studio_MixerStrip_GetPath(rawPtr, buffer, buffer.Length, out retrieved);

            if (result == RESULT.ERR_TRUNCATED)
            {
                buffer = new byte[retrieved];
                result = FMOD_Studio_MixerStrip_GetPath(rawPtr, buffer, buffer.Length, out retrieved);
            }

            if (result == RESULT.OK)
            {
                path = Encoding.UTF8.GetString(buffer, 0, retrieved - 1);
            }

            return result;
        }
        public RESULT getFaderLevel(out float volume)
        {
            return FMOD_Studio_MixerStrip_GetFaderLevel(rawPtr, out volume);
        }
        public RESULT setFaderLevel(float volume)
        {
            return FMOD_Studio_MixerStrip_SetFaderLevel(rawPtr, volume);
        }
        public RESULT getPaused(out bool paused)
        {
            RESULT result;
            int p = 0;

            result = FMOD_Studio_MixerStrip_GetPaused(rawPtr, out p);

            paused = (p != 0);

            return result;
        }
        public RESULT setPaused(bool paused)
        {
            return FMOD_Studio_MixerStrip_SetPaused(rawPtr, (paused ? 1 : 0));
        }
        public RESULT stopAllEvents(STOP_MODE mode)
        {
            return FMOD_Studio_MixerStrip_StopAllEvents(rawPtr, mode);
        }
        public RESULT getLoadingState(out LOADING_STATE state)
        {
            return FMOD_Studio_MixerStrip_GetLoadingState(rawPtr, out state);
        }
        public RESULT release()
        {
            return FMOD_Studio_MixerStrip_Release(rawPtr);
        }

        #region importfunctions

        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_MixerStrip_GetID           (IntPtr strip, out GUID id);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_MixerStrip_GetPath         (IntPtr strip, [Out] byte[] path, int size, out int retrieved);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_MixerStrip_GetFaderLevel   (IntPtr strip, out float value);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_MixerStrip_SetFaderLevel   (IntPtr strip, float value);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_MixerStrip_GetPaused       (IntPtr strip, out int paused);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_MixerStrip_SetPaused       (IntPtr strip, int paused);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_MixerStrip_StopAllEvents   (IntPtr strip, STOP_MODE mode);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_MixerStrip_GetLoadingState (IntPtr strip, out LOADING_STATE state);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_MixerStrip_Release         (IntPtr strip);

        #endregion
        #region wrapperinternal

        public MixerStrip(IntPtr raw)
        : base(raw)
        {
        }

        #endregion
    }

    public class Bank : HandleBase
    {
        // Property access
        
        public RESULT getID(out GUID id)
        {
            return FMOD_Studio_Bank_GetID(rawPtr, out id);
        }

        public RESULT getPath(out string path)
        {
            path = null;

            byte[] buffer = new byte[256];
            int retrieved = 0;
            RESULT result = FMOD_Studio_Bank_GetPath(rawPtr, buffer, buffer.Length, out retrieved);

            if (result == RESULT.ERR_TRUNCATED)
            {
                buffer = new byte[retrieved];
                result = FMOD_Studio_Bank_GetPath(rawPtr, buffer, buffer.Length, out retrieved);
            }

            if (result == RESULT.OK)
            {
                path = Encoding.UTF8.GetString(buffer, 0, retrieved - 1);
            }

            return result;
        }

        public RESULT unload()
        {
            RESULT result = FMOD_Studio_Bank_Unload(rawPtr);

            if (result != RESULT.OK)
            {
                return result;
            }

            rawPtr = IntPtr.Zero;
                
            return RESULT.OK;
        }

        public RESULT loadSampleData()
        {
            return FMOD_Studio_Bank_LoadSampleData(rawPtr);
        }

        public RESULT unloadSampleData()
        {
            return FMOD_Studio_Bank_UnloadSampleData(rawPtr);
        }

        public RESULT getLoadingState(out LOADING_STATE state)
        {
            return FMOD_Studio_Bank_GetLoadingState(rawPtr, out state);
        }
        public RESULT getSampleLoadingState(out LOADING_STATE state)
        {
            return FMOD_Studio_Bank_GetSampleLoadingState(rawPtr, out state);
        }

        // Enumeration
        public RESULT getEventCount(out int count)
        {
            return FMOD_Studio_Bank_GetEventCount(rawPtr, out count);
        }
        public RESULT getEventList(out EventDescription[] array)
        {
            array = null;

            RESULT result;
            int capacity;
            result = FMOD_Studio_Bank_GetEventCount(rawPtr, out capacity);
            if (result != RESULT.OK)
            {
                return result;
            }
            if (capacity == 0)
            {
                array = new EventDescription[0];
                return result;
            }

            IntPtr[] rawArray = new IntPtr[capacity];
            int actualCount;
            result = FMOD_Studio_Bank_GetEventList(rawPtr, rawArray, capacity, out actualCount);
            if (result != RESULT.OK)
            {
                return result;
            }
            if (actualCount > capacity) // More items added since we queried just now?
            {
                actualCount = capacity;
            }
            array = new EventDescription[actualCount];
            for (int i=0; i<actualCount; ++i)
            {
                array[i] = new EventDescription(rawArray[i]);
            }
            return RESULT.OK;
        }

        public RESULT getMixerStripCount(out int count)
        {
            return FMOD_Studio_Bank_GetMixerStripCount(rawPtr, out count);
        }
        public RESULT getMixerStripList(out MixerStrip[] array)
        {
            array = null;

            RESULT result;
            int capacity;
            result = FMOD_Studio_Bank_GetMixerStripCount(rawPtr, out capacity);
            if (result != RESULT.OK)
            {
                return result;
            }
            if (capacity == 0)
            {
                array = new MixerStrip[0];
                return result;
            }

            IntPtr[] rawArray = new IntPtr[capacity];
            int actualCount;
            result = FMOD_Studio_Bank_GetMixerStripList(rawPtr, rawArray, capacity, out actualCount);
            if (result != RESULT.OK)
            {
                return result;
            }
            if (actualCount > capacity) // More items added since we queried just now?
            {
                actualCount = capacity;
            }
            array = new MixerStrip[actualCount];
            for (int i=0; i<actualCount; ++i)
            {
                array[i] = new MixerStrip(rawArray[i]);
            }
            return RESULT.OK;
        }

        #region importfunctions

        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_Bank_GetID(IntPtr bank, out GUID id);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_Bank_GetPath(IntPtr bank, [Out] byte[] path, int size, out int retrieved);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_Bank_Unload(IntPtr bank);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_Bank_LoadSampleData(IntPtr bank);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_Bank_UnloadSampleData(IntPtr bank);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_Bank_GetLoadingState(IntPtr bank, out LOADING_STATE state);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_Bank_GetSampleLoadingState(IntPtr bank, out LOADING_STATE state);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_Bank_GetEventCount(IntPtr bank, out int count);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_Bank_GetEventList(IntPtr bank, IntPtr[] array, int capacity, out int count);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_Bank_GetMixerStripCount(IntPtr bank, out int count);
        [DllImport(STUDIO_VERSION.dll)]
        private static extern RESULT FMOD_Studio_Bank_GetMixerStripList(IntPtr bank, IntPtr[] array, int capacity, out int count);

        #endregion
        #region wrapperinternal

        public Bank(IntPtr raw)
        : base(raw)
        {
        }

        #endregion
    }
} // System
        
} // FMOD
