// Copyright 2010, Camilo Aguilar. Cloudescape, LLC.
#ifndef SRC_DOMAIN_H_
#define SRC_DOMAIN_H_

#include "node_libvirt.h"
#include "hypervisor.h"
#include "error.h"
#include "worker.h"

namespace NodeLibvirt {

    class Domain : public ObjectWrap {
        friend class Hypervisor;
        friend class DomainCreateWorker;
        friend class DomainDefineWorker;
        friend class DomainLookupByNameWorker;
        friend class DomainLookupByIdWorker;
        friend class DomainMigrateWorker;

        public:
            static void Initialize();
            static inline bool HasInstance(v8::Handle<v8::Value> value) {
                if (!value->IsObject()) {
                    return false;
                }
                v8::Local<v8::Object> object = value->ToObject();
                return constructor_template->HasInstance(object);
            }
            virtual ~Domain() {
                if (domain_ != NULL) {
                    virDomainFree(domain_);
                }
            }
            virDomainPtr domain() const;
        protected:
            static NAN_METHOD(Create);
            static NAN_METHOD(Define);
            static NAN_METHOD(LookupById);
            static NAN_METHOD(LookupByName);
            static Handle<Value> LookupByUUID(const Arguments& args);
            static Handle<Value> Undefine(const Arguments& args);
            static NAN_METHOD(GetId);
            static NAN_METHOD(GetInfo);
            static NAN_METHOD(GetName);
            static NAN_METHOD(GetUUID);
            static NAN_METHOD(GetAutostart);
            static NAN_METHOD(SetAutostart);
            static NAN_METHOD(GetOsType);
            static NAN_METHOD(GetMaxMemory);
            static NAN_METHOD(SetMaxMemory);
            static NAN_METHOD(SetMemory);
            static NAN_METHOD(GetMaxVcpus);
            static NAN_METHOD(IsActive);
            static NAN_METHOD(IsPersistent);
            static NAN_METHOD(IsUpdated);
            static NAN_METHOD(Reboot);
            static NAN_METHOD(Reset);
            static NAN_METHOD(Save);
            static NAN_METHOD(Restore);
            static NAN_METHOD(Suspend);
            static NAN_METHOD(Resume);
            static NAN_METHOD(Shutdown);
            static NAN_METHOD(Start);
            static Handle<Value> Destroy(const Arguments& args);
            static Handle<Value> SendKey(const Arguments& args);
            static NAN_METHOD(GetVcpus);
            static NAN_METHOD(SetVcpus);
            static NAN_METHOD(Migrate);
            static NAN_METHOD(SetMigrationMaxDowntime);
            static NAN_METHOD(PinVcpu);
            static NAN_METHOD(AttachDevice);
            static NAN_METHOD(DetachDevice);
            static NAN_METHOD(UpdateDevice);
            static NAN_METHOD(ToXml);
            static NAN_METHOD(GetJobInfo);
            static NAN_METHOD(AbortCurrentJob);
            static NAN_METHOD(GetSchedType); // TODO
            static NAN_METHOD(GetSchedParams);
            static NAN_METHOD(SetSchedParams);
            static NAN_METHOD(GetSecurityLabel);
            static NAN_METHOD(SaveManagedImage);
            static NAN_METHOD(RemoveManagedImage);
            static NAN_METHOD(HasManagedImage);
            static NAN_METHOD(MemoryPeek);
            static NAN_METHOD(GetMemoryStats);
            static NAN_METHOD(BlockPeek);
            static NAN_METHOD(GetBlockStats);
            static NAN_METHOD(GetBlockInfo);
            static NAN_METHOD(CoreDump);
            static NAN_METHOD(GetInterfaceStats);
            static NAN_METHOD(HasCurrentSnapshot);
            static NAN_METHOD(RevertToSnapshot);
            static NAN_METHOD(TakeSnapshot);
            static NAN_METHOD(GetCurrentSnapshot);
            static NAN_METHOD(DeleteSnapshot);
            static NAN_METHOD(LookupSnapshotByName);
            static NAN_METHOD(GetSnapshots);

        private:
            virDomainPtr domain_;
            static Persistent<FunctionTemplate> constructor_template;
    };

    class DomainCreateWorker : public LibvirtWorker {
        public:
            DomainCreateWorker(NanCallback *callback, virConnectPtr conn, const char *xml, unsigned int flags = 0)
                : LibvirtWorker(callback, conn), xml_(xml), flags_(flags) {}
            void Execute();
        protected:
            void HandleOKCallback();
        private:
            std::string xml_;
            unsigned int flags_;
            virDomainPtr domainptr_;
    };

    class DomainDefineWorker : public LibvirtWorker {
        public:
            DomainDefineWorker(NanCallback *callback, virConnectPtr conn, const char *xml, unsigned int flags = 0)
                : LibvirtWorker(callback, conn), xml_(xml), flags_(flags) {}
            void Execute();
        protected:
            void HandleOKCallback();
        private:
            std::string xml_;
            unsigned int flags_;
            virDomainPtr domainptr_;
    };

    class DomainLookupByIdWorker : public LibvirtWorker {
        public:
            DomainLookupByIdWorker(NanCallback *callback, virConnectPtr conn, int id)
                : LibvirtWorker(callback, conn), id_(id) {}

            void Execute();
        protected:
            void HandleOKCallback();
        private:
            int id_;
            virDomainPtr domainptr_;
    };

    class DomainLookupByNameWorker : public LibvirtWorker {
        public:
            DomainLookupByNameWorker(NanCallback *callback, virConnectPtr conn, char *name)
                : LibvirtWorker(callback, conn), name_(name) {}

            void Execute();
        protected:
            void HandleOKCallback();
        private:
            std::string name_;
            virDomainPtr domainptr_;
    };

    class DomainWorker : public LibvirtWorker {
        public:
            DomainWorker(NanCallback *callback, virDomainPtr domainptr)
                : LibvirtWorker(callback, associateConn(domainptr)), domainptr_(domainptr) {
            }

            virDomainPtr getDomainPtr() { return domainptr_; }
        private:
            virConnectPtr associateConn(virDomainPtr domainptr) {
                return virDomainGetConnect(domainptr);
            }

            virDomainPtr domainptr_;
    };

    class DomainGetIdWorker : public HelperWorker<DomainWorker, virDomainPtr, int, Integer> {
        public:
            DomainGetIdWorker(NanCallback *callback, virDomainPtr domainptr)
                : HelperWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainGetInfoWorker : public DomainWorker {
        public:
            DomainGetInfoWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr), info_(NULL) {
            }

            void Execute();
        protected:
            void HandleOKCallback();
        private:
            virDomainInfoPtr info_;
    };

    class DomainGetNameWorker : public StringReturnWorker<DomainWorker, virDomainPtr> {
        public:
            DomainGetNameWorker(NanCallback *callback, virDomainPtr domainptr)
                : StringReturnWorker(callback, domainptr, false) {}
            void Execute();
    };

    class DomainGetUUIDWorker : public StringReturnWorker<DomainWorker, virDomainPtr> {
        public:
            DomainGetUUIDWorker(NanCallback *callback, virDomainPtr domainptr)
                : StringReturnWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainGetAutostartWorker : public BooleanReturnWorker<DomainWorker, virDomainPtr> {
        public:
            DomainGetAutostartWorker(NanCallback *callback, virDomainPtr domainptr)
                : BooleanReturnWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainSetAutostartWorker : public BooleanReturnWorker<DomainWorker, virDomainPtr> {
        public:
            DomainSetAutostartWorker(NanCallback *callback, virDomainPtr domainptr, int autostart)
                : BooleanReturnWorker(callback, domainptr), autostart_(autostart) {}
            void Execute();
        private:
            int autostart_;
    };

    class DomainGetOsTypeWorker : public StringReturnWorker<DomainWorker, virDomainPtr> {
        public:
            DomainGetOsTypeWorker(NanCallback *callback, virDomainPtr domainptr)
                : StringReturnWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainGetMaxMemoryWorker : public HelperWorker<DomainWorker, virDomainPtr, unsigned long, Number> {
        public:
            DomainGetMaxMemoryWorker(NanCallback *callback, virDomainPtr domainptr)
                : HelperWorker(callback, domainptr, 0L) {}
            void Execute();
    };

    class DomainSetMaxMemoryWorker : public DomainWorker {
        public:
            DomainSetMaxMemoryWorker(NanCallback *callback, virDomainPtr domainptr, unsigned long maxmem)
                : DomainWorker(callback, domainptr), maxmem_(maxmem) {}
            void Execute();
        private:
            unsigned long maxmem_;
    };

    class DomainSetMemoryWorker : public DomainWorker {
        public:
            DomainSetMemoryWorker(NanCallback *callback, virDomainPtr domainptr, unsigned long mem)
                : DomainWorker(callback, domainptr), mem_(mem) {}
            void Execute();
        private:
            unsigned long mem_;
    };

    class DomainGetMaxVcpusWorker : public HelperWorker<DomainWorker, virDomainPtr, int, Integer> {
        public:
            DomainGetMaxVcpusWorker(NanCallback *callback, virDomainPtr domainptr)
                : HelperWorker(callback, domainptr, -1) {}
            void Execute();
    };

    class DomainIsActiveWorker : public BooleanReturnWorker<DomainWorker, virDomainPtr> {
        public:
            DomainIsActiveWorker(NanCallback *callback, virDomainPtr domainptr)
                : BooleanReturnWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainIsPersistentWorker : public BooleanReturnWorker<DomainWorker, virDomainPtr> {
        public:
            DomainIsPersistentWorker(NanCallback *callback, virDomainPtr domainptr)
                : BooleanReturnWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainIsUpdatedWorker : public BooleanReturnWorker<DomainWorker, virDomainPtr> {
        public:
            DomainIsUpdatedWorker(NanCallback *callback, virDomainPtr domainptr)
                : BooleanReturnWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainRebootWorker : public DomainWorker {
        public:
            DomainRebootWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainResetWorker : public DomainWorker {
        public:
            DomainResetWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainSaveWorker : public DomainWorker {
        public:
            DomainSaveWorker(NanCallback *callback, virDomainPtr domainptr, char *path)
                : DomainWorker(callback, domainptr), path_(path) {}
            void Execute();
        private:
            std::string path_;
    };

    class DomainRestoreWorker : public LibvirtWorker {
        public:
            DomainRestoreWorker(NanCallback *callback, virConnectPtr conn, char *path)
                : LibvirtWorker(callback, conn), path_(path) {}
            void Execute();
        private:
            std::string path_;
    };

    class DomainSuspendWorker : public DomainWorker {
        public:
            DomainSuspendWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainResumeWorker : public DomainWorker {
        public:
            DomainResumeWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainShutdownWorker : public DomainWorker {
        public:
            DomainShutdownWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainStartWorker : public DomainWorker {
        public:
            DomainStartWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainGetVcpusWorker : public DomainWorker {
        public:
            DomainGetVcpusWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr) {
            }

            void Execute();
        protected:
            void HandleOKCallback();
        private:
            virDomainInfo info_;
            virNodeInfo nodeinfo_;
            int cpumaplen_;
            std::vector<virVcpuInfo> cpuinfo_;
            std::vector<unsigned char> cpumap_;
    };

    class DomainSetVcpusWorker : public DomainWorker {
        public:
            DomainSetVcpusWorker(NanCallback *callback, virDomainPtr domainptr, unsigned int vcpus)
                : DomainWorker(callback, domainptr), vcpus_(vcpus) {}
            void Execute();
        private:
            unsigned int vcpus_;
    };

    class DomainMigrateWorker : public DomainWorker {
        public:
            DomainMigrateWorker(NanCallback *callback, virDomainPtr domainptr, char *uri)
                : DomainWorker(callback, domainptr), uri_(uri), conn_(NULL), migrated_(NULL), flags_(0), bandwidth_(0) {}
            DomainMigrateWorker(NanCallback *callback, virDomainPtr domainptr, virConnectPtr conn)
                : DomainWorker(callback, domainptr), conn_(conn), migrated_(NULL), flags_(0), bandwidth_(0) {}
            void Execute();
            void setFlags(unsigned long flags) { flags_ = flags; }
            void setBandwidth(unsigned long bandwidth) { bandwidth_ = bandwidth; }
            void setDestname(const char *destname) { destname_ = destname; }
        protected:
            void HandleOKCallback();
        private:
            std::string uri_;
            virConnectPtr conn_;
            virDomainPtr migrated_;
            std::string destname_;
            unsigned long flags_;
            unsigned long bandwidth_;
    };

    class DomainSetMigrationMaxDowntimeWorker : public DomainWorker {
        public:
            DomainSetMigrationMaxDowntimeWorker(NanCallback *callback, virDomainPtr domainptr, long long downtime, unsigned int flags)
                : DomainWorker(callback, domainptr), downtime_(downtime), flags_(flags) {}
            void Execute();
        private:
            long long downtime_;
            unsigned int flags_;
    };

    class DomainPinVcpuWorker : public DomainWorker {
        public:
            DomainPinVcpuWorker(NanCallback *callback, virDomainPtr domainptr, int vcpu, Handle<Array> cpus);
            void Execute();
        private:
            int vcpu_;
            int length_;
            std::vector<bool> usables_;
            std::vector<int> vcpus_;
    };

    class DomainAttachDeviceWorker : public DomainWorker {
        public:
            DomainAttachDeviceWorker(NanCallback *callback, virDomainPtr domainptr, char *xml, unsigned int flags = 0)
                : DomainWorker(callback, domainptr), xml_(xml), flags_(flags) {}
            void Execute();
        private:
            std::string xml_;
            unsigned int flags_;
    };

    class DomainDetachDeviceWorker : public DomainWorker {
        public:
            DomainDetachDeviceWorker(NanCallback *callback, virDomainPtr domainptr, char *xml, unsigned int flags = 0)
                : DomainWorker(callback, domainptr), xml_(xml), flags_(flags) {}
            void Execute();
        private:
            std::string xml_;
            unsigned int flags_;
    };

    class DomainUpdateDeviceWorker : public DomainWorker {
        public:
            DomainUpdateDeviceWorker(NanCallback *callback, virDomainPtr domainptr, char *xml, unsigned int flags = 0)
                : DomainWorker(callback, domainptr), xml_(xml), flags_(flags) {}
            void Execute();
        private:
            std::string xml_;
            unsigned int flags_;
    };

    class DomainToXmlWorker : public StringReturnWorker<DomainWorker, virDomainPtr> {
        public:
            DomainToXmlWorker(NanCallback *callback, virDomainPtr domainptr, int flags)
                : StringReturnWorker(callback, domainptr), flags_(flags) {}
            void Execute();
        private:
            int flags_;
    };

    class DomainGetJobInfoWorker : public DomainWorker {
        public:
            DomainGetJobInfoWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr) {}
            void Execute();
        protected:
            void HandleOKCallback();
        private:
            virDomainJobInfo info_;
    };

    class DomainAbortCurrentJobWorker : public DomainWorker {
        public:
            DomainAbortCurrentJobWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainGetSchedParamsWorker : public DomainWorker {
        public:
            DomainGetSchedParamsWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr) {}
            void Execute();
        protected:
            void HandleOKCallback();
        private:
            std::vector<virSchedParameter> params_;
    };

    class DomainSetSchedParamsWorker : public DomainWorker {
        public:
            DomainSetSchedParamsWorker(NanCallback *callback, virDomainPtr domainptr, Handle<Object> newparams);
            void Execute();
        private:
            std::vector<virSchedParameter> newparams_;
    };

    class DomainGetSecurityLabelWorker : public DomainWorker {
        public:
            DomainGetSecurityLabelWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr) {}
            void Execute();
        protected:
            void HandleOKCallback();
        private:
            virSecurityLabel label_;
    };

    class DomainSaveManagedImageWorker : public DomainWorker {
        public:
            DomainSaveManagedImageWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainRemoveManagedImageWorker : public DomainWorker {
        public:
            DomainRemoveManagedImageWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainHasManagedImageWorker : public BooleanReturnWorker<DomainWorker, virDomainPtr> {
        public:
            DomainHasManagedImageWorker(NanCallback *callback, virDomainPtr domainptr)
                : BooleanReturnWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainMemoryPeekWorker : public DomainWorker {
        public:
            DomainMemoryPeekWorker(NanCallback *callback, virDomainPtr domainptr, unsigned long long start, size_t size, unsigned int flags)
                : DomainWorker(callback, domainptr), start_(start), size_(size), flags_(flags), buffer_(size) {}
            void Execute();
        protected:
            void HandleOKCallback();
        private:
            unsigned long long start_;
            size_t size_;
            unsigned int flags_;
            std::vector<char> buffer_;
    };

    class DomainGetMemoryStatsWorker : public DomainWorker {
        public:
            DomainGetMemoryStatsWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr), stats_(VIR_DOMAIN_MEMORY_STAT_NR) {}
            void Execute();
        protected:
            void HandleOKCallback();
        private:
            std::vector<virDomainMemoryStatStruct> stats_;
    };

    class DomainBlockPeekWorker : public DomainWorker {
        public:
            DomainBlockPeekWorker(NanCallback *callback, virDomainPtr domainptr, const char *path, unsigned long long start, size_t size, unsigned int flags)
                : DomainWorker(callback, domainptr), path_(path), start_(start), size_(size), flags_(flags), buffer_(size) {}
            void Execute();
        protected:
            void HandleOKCallback();
        private:
            std::string path_;
            unsigned long long start_;
            size_t size_;
            unsigned int flags_;
            std::vector<char> buffer_;
    };

    class DomainGetBlockStatsWorker : public DomainWorker {
        public:
            DomainGetBlockStatsWorker(NanCallback *callback, virDomainPtr domainptr, const char *path)
                : DomainWorker(callback, domainptr), path_(path) {}
            void Execute();
        protected:
            void HandleOKCallback();
        private:
            std::string path_;
            virDomainBlockStatsStruct stats_;
    };

    class DomainGetBlockInfoWorker : public DomainWorker {
        public:
            DomainGetBlockInfoWorker(NanCallback *callback, virDomainPtr domainptr, const char *path)
                : DomainWorker(callback, domainptr), path_(path) {}
            void Execute();
        protected:
            void HandleOKCallback();
        private:
            std::string path_;
            virDomainBlockInfo info_;
    };

    class DomainCoreDumpWorker : public DomainWorker {
        public:
            DomainCoreDumpWorker(NanCallback *callback, virDomainPtr domainptr, const char *path)
                : DomainWorker(callback, domainptr), path_(path) {}
            void Execute();
        private:
            std::string path_;
    };

    class DomainGetInterfaceStatsWorker : public DomainWorker {
        public:
            DomainGetInterfaceStatsWorker(NanCallback *callback, virDomainPtr domainptr, const char *device)
                : DomainWorker(callback, domainptr), device_(device) {}
            void Execute();
        protected:
            void HandleOKCallback();
        private:
            std::string device_;
            struct _virDomainInterfaceStats stats_;
    };

    class DomainHasCurrentSnapshotWorker : public BooleanReturnWorker<DomainWorker, virDomainPtr> {
        public:
            DomainHasCurrentSnapshotWorker(NanCallback *callback, virDomainPtr domainptr)
                : BooleanReturnWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainRevertToSnapshotWorker : public DomainWorker {
        public:
            DomainRevertToSnapshotWorker(NanCallback *callback, virDomainPtr domainptr, const char *name)
                : DomainWorker(callback, domainptr), name_(name) {}
            void Execute();
        private:
            std::string name_;
    };

    class DomainTakeSnapshotWorker : public DomainWorker {
        public:
            DomainTakeSnapshotWorker(NanCallback *callback, virDomainPtr domainptr, const char *xml)
                : DomainWorker(callback, domainptr), xml_(xml) {}
            DomainTakeSnapshotWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr) {}
            void Execute();
        private:
            std::string xml_;
    };

    class DomainGetCurrentSnapshotWorker : public StringReturnWorker<DomainWorker, virDomainPtr> {
        public:
            DomainGetCurrentSnapshotWorker(NanCallback *callback, virDomainPtr domainptr)
                : StringReturnWorker(callback, domainptr) {}
            void Execute();
    };

    class DomainDeleteSnapshotWorker : public DomainWorker {
        public:
            DomainDeleteSnapshotWorker(NanCallback *callback, virDomainPtr domainptr, const char *name)
                : DomainWorker(callback, domainptr), name_(name) {}
            void Execute();
        private:
            std::string name_;
    };

    class DomainLookupSnapshotByNameWorker : public StringReturnWorker<DomainWorker, virDomainPtr> {
        public:
            DomainLookupSnapshotByNameWorker(NanCallback *callback, virDomainPtr domainptr, const char *name)
                : StringReturnWorker(callback, domainptr), name_(name) {}
            void Execute();
        private:
            std::string name_;
    };

    class DomainGetSnapshotsWorker : public DomainWorker {
        public:
            DomainGetSnapshotsWorker(NanCallback *callback, virDomainPtr domainptr)
                : DomainWorker(callback, domainptr) {}
            void Execute();
        protected:
            void HandleOKCallback();
        private:
            std::vector<std::string> xmls_;
    };

}  //namespace NodeLibvirt

#endif  // SRC_DOMAIN_H
