// Copyright 2010, Camilo Aguilar. Cloudescape, LLC.
#include <stdlib.h>
#include <string.h>
#include <node_buffer.h>
#include "domain.h"

#define NOARGS_WORKER_METHOD(name, worker)                                  \
                                                                            \
NAN_METHOD(Domain::name) {                                                          \
  NanScope();                                                               \
                                                                            \
  Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());                 \
                                                                            \
  if (args.Length() != 1 || !args[0]->IsFunction()) {                       \
    return ThrowException(Exception::TypeError(                             \
    String::New("You must specify a function as first argument")));         \
  }                                                                         \
                                                                            \
  NanCallback *callback = new NanCallback(args[0].As<Function>());          \
                                                                            \
  NanAsyncQueueWorker(new worker(callback, domain->domain_));               \
                                                                            \
  NanReturnUndefined();                                                     \
}

namespace NodeLibvirt {
    Persistent<FunctionTemplate> Domain::constructor_template;

    static Persistent<String> state_symbol;
    static Persistent<String> max_memory_symbol;
    static Persistent<String> memory_symbol;
    static Persistent<String> vcpus_number_symbol;
    static Persistent<String> cpu_time_symbol;
    static Persistent<String> number_symbol;
    static Persistent<String> cpu_symbol;
    static Persistent<String> affinity_symbol;
    static Persistent<String> usable_symbol;
    static Persistent<String> migration_uri_symbol;
    static Persistent<String> migration_name_symbol;
    static Persistent<String> migration_bandwidth_symbol;
    static Persistent<String> migration_flags_symbol;
    static Persistent<String> migration_hypervisor_symbol;

    //Jobinfo symbols
    static Persistent<String> type_symbol;
    static Persistent<String> time_symbol;
    static Persistent<String> data_symbol;
    static Persistent<String> file_symbol;
    static Persistent<String> elapsed_symbol;
    static Persistent<String> remaining_symbol;
    static Persistent<String> total_symbol;
    static Persistent<String> processed_symbol;

    //SecurityLabel symbols
    static Persistent<String> label_symbol;
    static Persistent<String> enforcing_symbol;

    //memory stat symbols
    static Persistent<String> memory_stat_swap_in_symbol;
    static Persistent<String> memory_stat_swap_out_symbol;
    static Persistent<String> memory_stat_major_fault_symbol;
    static Persistent<String> memory_stat_minor_fault_symbol;
    static Persistent<String> memory_stat_unused_symbol;
    static Persistent<String> memory_stat_available_symbol;

    //block stat symbols
    static Persistent<String> block_stat_rd_req_symbol;
    static Persistent<String> block_stat_rd_bytes_symbol;
    static Persistent<String> block_stat_wr_req_symbol;
    static Persistent<String> block_stat_wr_bytes_symbol;
    static Persistent<String> block_stat_errs_symbol;

    //block info symbols
    static Persistent<String> block_info_capacity_symbol;
    static Persistent<String> block_info_allocation_symbol;
    static Persistent<String> block_info_physical_symbol;

    //domain network interface statistics symbols
    static Persistent<String> nwiface_stat_rx_bytes_symbol;
    static Persistent<String> nwiface_stat_rx_packets_symbol;
    static Persistent<String> nwiface_stat_rx_errors_symbol;
    static Persistent<String> nwiface_stat_rx_drop_symbol;
    static Persistent<String> nwiface_stat_tx_bytes_symbol;
    static Persistent<String> nwiface_stat_tx_packets_symbol;
    static Persistent<String> nwiface_stat_tx_errors_symbol;
    static Persistent<String> nwiface_stat_tx_drop_symbol;

    void Domain::Initialize() {
        Local<FunctionTemplate> t = FunctionTemplate::New();

//        t->Inherit(EventEmitter::constructor_template);
        t->InstanceTemplate()->SetInternalFieldCount(1);

        NODE_SET_PROTOTYPE_METHOD(t, "getId",
                                      Domain::GetId);
        NODE_SET_PROTOTYPE_METHOD(t, "toXml",
                                      Domain::ToXml);
        NODE_SET_PROTOTYPE_METHOD(t, "getJobInfo",
                                      Domain::GetJobInfo);
        NODE_SET_PROTOTYPE_METHOD(t, "abortCurrentJob",
                                      Domain::AbortCurrentJob);
        NODE_SET_PROTOTYPE_METHOD(t, "getMaxMemory",
                                      Domain::GetMaxMemory);
        NODE_SET_PROTOTYPE_METHOD(t, "setMaxMemory",
                                      Domain::SetMaxMemory);
        NODE_SET_PROTOTYPE_METHOD(t, "setMemory",
                                      Domain::SetMemory);
        NODE_SET_PROTOTYPE_METHOD(t, "getMaxVcpus",
                                      Domain::GetMaxVcpus);
        NODE_SET_PROTOTYPE_METHOD(t, "getAutostart",
                                      Domain::GetAutostart);
        NODE_SET_PROTOTYPE_METHOD(t, "setAutostart",
                                      Domain::SetAutostart);
        NODE_SET_PROTOTYPE_METHOD(t, "getName",
                                      Domain::GetName);
        NODE_SET_PROTOTYPE_METHOD(t, "getOsType",
                                      Domain::GetOsType);
        NODE_SET_PROTOTYPE_METHOD(t, "getInfo",
                                      Domain::GetInfo);
        NODE_SET_PROTOTYPE_METHOD(t, "getSchedParams",
                                      Domain::GetSchedParams);
        NODE_SET_PROTOTYPE_METHOD(t, "setSchedParams",
                                      Domain::SetSchedParams);
        /*NODE_SET_PROTOTYPE_METHOD(t, "getSchedType",
                                      Domain::GetSchedType); */ //It's necessary this function?
        NODE_SET_PROTOTYPE_METHOD(t, "getSecurityLabel",
                                      Domain::GetSecurityLabel);
        NODE_SET_PROTOTYPE_METHOD(t, "hasManagedImage",
                                      Domain::HasManagedImage);
        NODE_SET_PROTOTYPE_METHOD(t, "saveManagedImage",
                                      Domain::SaveManagedImage);
        NODE_SET_PROTOTYPE_METHOD(t, "removeManagedImage",
                                      Domain::RemoveManagedImage);
        NODE_SET_PROTOTYPE_METHOD(t, "memoryPeek",
                                      Domain::MemoryPeek);
        NODE_SET_PROTOTYPE_METHOD(t, "getMemoryStats",
                                      Domain::GetMemoryStats);
        NODE_SET_PROTOTYPE_METHOD(t, "blockPeek",
                                      Domain::BlockPeek);
        NODE_SET_PROTOTYPE_METHOD(t, "getBlockStats",
                                      Domain::GetBlockStats);
        NODE_SET_PROTOTYPE_METHOD(t, "getBlockInfo",
                                      Domain::GetBlockInfo);
        NODE_SET_PROTOTYPE_METHOD(t, "getUUID",
                                      Domain::GetUUID);
        NODE_SET_PROTOTYPE_METHOD(t, "getVcpus",
                                      Domain::GetVcpus);
        NODE_SET_PROTOTYPE_METHOD(t, "setVcpus",
                                      Domain::SetVcpus);
        NODE_SET_PROTOTYPE_METHOD(t, "isActive",
                                      Domain::IsActive);
        NODE_SET_PROTOTYPE_METHOD(t, "isPersistent",
                                      Domain::IsPersistent);
        NODE_SET_PROTOTYPE_METHOD(t, "isUpdated",
                                      Domain::IsUpdated);
        NODE_SET_PROTOTYPE_METHOD(t, "getInterfaceStats",
                                      Domain::GetInterfaceStats);
        NODE_SET_PROTOTYPE_METHOD(t, "coreDump",
                                      Domain::CoreDump);
        NODE_SET_PROTOTYPE_METHOD(t, "migrate",
                                      Domain::Migrate);
        NODE_SET_PROTOTYPE_METHOD(t, "setMigrationMaxDowntime",
                                      Domain::SetMigrationMaxDowntime);
        NODE_SET_PROTOTYPE_METHOD(t, "pinVcpu",
                                      Domain::PinVcpu);
        NODE_SET_PROTOTYPE_METHOD(t, "reboot",
                                      Domain::Reboot);
        NODE_SET_PROTOTYPE_METHOD(t, "reset",
                                      Domain::Reset);
        NODE_SET_PROTOTYPE_METHOD(t, "resume",
                                      Domain::Resume);
        NODE_SET_PROTOTYPE_METHOD(t, "save",
                                      Domain::Save);
        NODE_SET_PROTOTYPE_METHOD(t, "shutdown",
                                      Domain::Shutdown);
        NODE_SET_PROTOTYPE_METHOD(t, "start",
                                      Domain::Start);
        NODE_SET_PROTOTYPE_METHOD(t, "suspend",
                                      Domain::Suspend);
        NODE_SET_PROTOTYPE_METHOD(t, "sendKey",
                                      Domain::SendKey);
        NODE_SET_PROTOTYPE_METHOD(t, "attachDevice",
                                      Domain::AttachDevice);
        NODE_SET_PROTOTYPE_METHOD(t, "detachDevice",
                                      Domain::DetachDevice);
        NODE_SET_PROTOTYPE_METHOD(t, "updateDevice",
                                      Domain::UpdateDevice);
        NODE_SET_PROTOTYPE_METHOD(t, "destroy",
                                      Domain::Destroy);
        NODE_SET_PROTOTYPE_METHOD(t, "undefine",
                                      Domain::Undefine);
        NODE_SET_PROTOTYPE_METHOD(t, "hasCurrentSnapshot",
                                      Domain::HasCurrentSnapshot);
        NODE_SET_PROTOTYPE_METHOD(t, "revertToSnapshot",
                                      Domain::RevertToSnapshot);
        NODE_SET_PROTOTYPE_METHOD(t, "takeSnapshot",
                                      Domain::TakeSnapshot);
        NODE_SET_PROTOTYPE_METHOD(t, "getCurrentSnapshot",
                                      Domain::GetCurrentSnapshot);
        NODE_SET_PROTOTYPE_METHOD(t, "deleteSnapshot",
                                      Domain::DeleteSnapshot);
        NODE_SET_PROTOTYPE_METHOD(t, "lookupSnapshotByName",
                                      Domain::LookupSnapshotByName);
        NODE_SET_PROTOTYPE_METHOD(t, "getSnapshots",
                                      Domain::GetSnapshots);

        constructor_template = Persistent<FunctionTemplate>::New(t);
        constructor_template->SetClassName(String::NewSymbol("Domain"));

        Local<ObjectTemplate> object_tmpl = t->InstanceTemplate();

        //Constants initialization
        //virDomainState
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_NOSTATE);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_RUNNING);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_BLOCKED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_PAUSED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_SHUTDOWN);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_SHUTOFF);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_CRASHED);

#ifdef VIR_DOMAIN_PMSUSPENDED
        // If its available in libvirt.h, then make it available in node
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_PMSUSPENDED);
#endif

        //virDomainDeviceModifyFlags
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_DEVICE_MODIFY_CURRENT);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_DEVICE_MODIFY_LIVE);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_DEVICE_MODIFY_CONFIG);

        //virDomainMigrateFlags
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_MIGRATE_LIVE);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_MIGRATE_PEER2PEER);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_MIGRATE_TUNNELLED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_MIGRATE_PERSIST_DEST);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_MIGRATE_UNDEFINE_SOURCE);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_MIGRATE_PAUSED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_MIGRATE_NON_SHARED_DISK);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_MIGRATE_NON_SHARED_INC);

        //virDomainXMLFlags
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_XML_SECURE);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_XML_INACTIVE);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_XML_UPDATE_CPU);

        //virDomainJobType
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_JOB_NONE);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_JOB_BOUNDED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_JOB_UNBOUNDED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_JOB_COMPLETED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_JOB_FAILED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_JOB_CANCELLED);

        //virDomainMemoryFlags
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_MEMORY_VIRTUAL);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_MEMORY_PHYSICAL);

        //virDomainEventType
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_DEFINED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_UNDEFINED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_STARTED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_SUSPENDED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_RESUMED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_STOPPED);

        //virDomainEventIOErrorAction
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_IO_ERROR_NONE);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_IO_ERROR_PAUSE);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_IO_ERROR_REPORT);

        //virDomainEventResumedDetailType
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_RESUMED_UNPAUSED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_RESUMED_MIGRATED);

        //virDomainEventStartedDetailType
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_STARTED_BOOTED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_STARTED_MIGRATED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_STARTED_RESTORED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_STARTED_FROM_SNAPSHOT);

        //virDomainEventStoppedDetailType
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_STOPPED_SHUTDOWN);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_STOPPED_DESTROYED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_STOPPED_CRASHED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_STOPPED_MIGRATED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_STOPPED_SAVED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_STOPPED_FAILED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_STOPPED_FROM_SNAPSHOT);

        //virDomainEventSuspendedDetailType
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_SUSPENDED_PAUSED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_SUSPENDED_MIGRATED);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_SUSPENDED_IOERROR);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_SUSPENDED_WATCHDOG);

        //virDomainEventUndefinedDetailType
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_UNDEFINED_REMOVED);

        //virDomainEventWatchdogAction
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_WATCHDOG_NONE);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_WATCHDOG_PAUSE);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_WATCHDOG_RESET);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_WATCHDOG_POWEROFF);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_WATCHDOG_SHUTDOWN);
        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_EVENT_WATCHDOG_DEBUG);


        NODE_DEFINE_CONSTANT(object_tmpl, VIR_DOMAIN_SEND_KEY_MAX_KEYS);

        state_symbol        = NODE_PSYMBOL("state");
        max_memory_symbol   = NODE_PSYMBOL("max_memory");
        memory_symbol       = NODE_PSYMBOL("memory");
        vcpus_number_symbol = NODE_PSYMBOL("vcpus_number");
        cpu_time_symbol     = NODE_PSYMBOL("cpu_time");
        number_symbol       = NODE_PSYMBOL("number");
        cpu_symbol          = NODE_PSYMBOL("cpu");
        affinity_symbol     = NODE_PSYMBOL("affinity");
        usable_symbol       = NODE_PSYMBOL("usable");
        migration_uri_symbol            = NODE_PSYMBOL("dest_uri");
        migration_name_symbol           = NODE_PSYMBOL("dest_name");
        migration_bandwidth_symbol      = NODE_PSYMBOL("bandwidth");
        migration_flags_symbol          = NODE_PSYMBOL("flags");
        migration_hypervisor_symbol     = NODE_PSYMBOL("dest_hypervisor");

        type_symbol         = NODE_PSYMBOL("type");
        time_symbol         = NODE_PSYMBOL("time");
        data_symbol         = NODE_PSYMBOL("data");
        file_symbol         = NODE_PSYMBOL("file");
        elapsed_symbol      = NODE_PSYMBOL("elapsed");
        remaining_symbol    = NODE_PSYMBOL("remaining");
        total_symbol        = NODE_PSYMBOL("total");
        processed_symbol    = NODE_PSYMBOL("processed");

        label_symbol = NODE_PSYMBOL("label");
        enforcing_symbol = NODE_PSYMBOL("enforcing");

        memory_stat_swap_in_symbol = NODE_PSYMBOL("swap_in");
        memory_stat_swap_out_symbol = NODE_PSYMBOL("swap_out");
        memory_stat_major_fault_symbol = NODE_PSYMBOL("major_fault");
        memory_stat_minor_fault_symbol = NODE_PSYMBOL("minor_fault");
        memory_stat_unused_symbol = NODE_PSYMBOL("unused");
        memory_stat_available_symbol = NODE_PSYMBOL("available");

        block_stat_rd_req_symbol = NODE_PSYMBOL("read_requests");
        block_stat_rd_bytes_symbol = NODE_PSYMBOL("read_bytes");
        block_stat_wr_req_symbol = NODE_PSYMBOL("write_requests");
        block_stat_wr_bytes_symbol = NODE_PSYMBOL("write_bytes");
        block_stat_errs_symbol = NODE_PSYMBOL("errors");

        block_info_capacity_symbol = NODE_PSYMBOL("capacity");
        block_info_allocation_symbol = NODE_PSYMBOL("allocation");
        block_info_physical_symbol = NODE_PSYMBOL("physical");

        nwiface_stat_rx_bytes_symbol = NODE_PSYMBOL("rx_bytes");
        nwiface_stat_rx_packets_symbol = NODE_PSYMBOL("rx_packets");
        nwiface_stat_rx_errors_symbol = NODE_PSYMBOL("rx_errors");
        nwiface_stat_rx_drop_symbol = NODE_PSYMBOL("rx_drop");
        nwiface_stat_tx_bytes_symbol = NODE_PSYMBOL("tx_bytes");
        nwiface_stat_tx_packets_symbol = NODE_PSYMBOL("tx_packets");
        nwiface_stat_tx_errors_symbol = NODE_PSYMBOL("tx_errors");
        nwiface_stat_tx_drop_symbol = NODE_PSYMBOL("tx_drop");
    }

    virDomainPtr Domain::domain() const {
        return domain_;
    }

    void DomainCreateWorker::Execute() {
        domainptr_ = virDomainCreateXML(getConnection(), xml_.c_str(), flags_);

        if (domainptr_ == NULL) {
            setVirError(virGetLastError());
        }
    }

    void DomainCreateWorker::HandleOKCallback() {
        NanScope();

        Domain *domain = new Domain();
        domain->domain_ = domainptr_;

        Local<Object> domain_obj = domain->constructor_template->GetFunction()->NewInstance();
        domain->Wrap(domain_obj);

        Local<Value> argv[] = { NanNull(), domain_obj };

        callback->Call(2, argv);
    }

    NAN_METHOD(Domain::Create) {
        NanScope();
        unsigned int flags = 0;

        int argsl = args.Length();

        if(argsl != 2 || !args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a string as first argument")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        Local<Object> hyp_obj = args.This();

        if(!Hypervisor::HasInstance(hyp_obj)) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a Hypervisor object instance")));
        }
        String::Utf8Value xml(args[0]->ToString());

        Hypervisor *hypervisor = ObjectWrap::Unwrap<Hypervisor>(hyp_obj);
        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainCreateWorker(callback, hypervisor->connection(), *xml, flags));

        NanReturnUndefined();
    }

    void DomainDefineWorker::Execute() {
        domainptr_ = virDomainDefineXML(getConnection(), xml_.c_str());

        if (domainptr_ == NULL) {
            setVirError(virGetLastError());
        }
    }

    void DomainDefineWorker::HandleOKCallback() {
        NanScope();

        Domain *domain = new Domain();
        domain->domain_ = domainptr_;

        Local<Object> domain_obj = domain->constructor_template->GetFunction()->NewInstance();
        domain->Wrap(domain_obj);

        Local<Value> argv[] = { NanNull(), domain_obj };

        callback->Call(2, argv);
    }

    NAN_METHOD(Domain::Define) {
        NanScope();
        unsigned int flags = 0;

        int argsl = args.Length();

        if(argsl != 2 || !args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a string as first argument")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        Local<Object> hyp_obj = args.This();

        if(!Hypervisor::HasInstance(hyp_obj)) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a Hypervisor object instance")));
        }
        String::Utf8Value xml(args[0]->ToString());

        Hypervisor *hypervisor = ObjectWrap::Unwrap<Hypervisor>(hyp_obj);
        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainDefineWorker(callback, hypervisor->connection(), *xml, flags));

        NanReturnUndefined();
    }

    Handle<Value> Domain::Undefine(const Arguments& args) {
        HandleScope scope;
        int ret = -1;

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
        ret = virDomainUndefine(domain->domain_);

        if(ret == -1) {
            ThrowException(Error::New(virGetLastError()));
            return False();
        }

        return True();
    }

    void DomainLookupByIdWorker::Execute() {
        domainptr_ = virDomainLookupByID(getConnection(), id_);

        if(domainptr_ == NULL) {
            setVirError(virGetLastError());
            return;
        }
    }

    void DomainLookupByIdWorker::HandleOKCallback() {
        NanScope();

        Domain *domain = new Domain();
        domain->domain_ = domainptr_;

        Local<Object> domain_obj = domain->constructor_template->GetFunction()->NewInstance();
        domain->Wrap(domain_obj);

        Local<Value> argv[] = { NanNull(), domain_obj };

        callback->Call(2, argv);
    }

    NAN_METHOD(Domain::LookupById) {
        NanScope();

        int id = -1;

        if(args.Length() != 2 || !args[0]->IsInt32()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a valid Domain Id.")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        Local<Object> hyp_obj = args.This();

        if(!Hypervisor::HasInstance(hyp_obj)) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a Hypervisor instance")));
        }

        id = args[0]->Int32Value();
        String::Utf8Value name(args[0]->ToString());

        Hypervisor *hypervisor = ObjectWrap::Unwrap<Hypervisor>(hyp_obj);
        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainLookupByIdWorker(callback, hypervisor->connection(), id));

        NanReturnUndefined();
    }

    void DomainLookupByNameWorker::Execute() {
        domainptr_ = virDomainLookupByName(getConnection(), name_.c_str());

        if(domainptr_ == NULL) {
            setVirError(virGetLastError());
            return;
        }
    }

    void DomainLookupByNameWorker::HandleOKCallback() {
        NanScope();

        Domain *domain = new Domain();
        domain->domain_ = domainptr_;

        Local<Object> domain_obj = domain->constructor_template->GetFunction()->NewInstance();
        domain->Wrap(domain_obj);

        Local<Value> argv[] = { NanNull(), domain_obj };

        callback->Call(2, argv);
    }

    NAN_METHOD(Domain::LookupByName) {
        NanScope();

        if(args.Length() != 2 || !args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a valid Domain name.")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        Local<Object> hyp_obj = args.This();

        if(!Hypervisor::HasInstance(hyp_obj)) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a Hypervisor instance")));
        }

        String::Utf8Value name(args[0]->ToString());

        Hypervisor *hypervisor = ObjectWrap::Unwrap<Hypervisor>(hyp_obj);
        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainLookupByNameWorker(callback, hypervisor->connection(), (char*)*name));

        NanReturnUndefined();
    }

    Handle<Value> Domain::LookupByUUID(const Arguments& args) {
        HandleScope scope;

        if(args.Length() == 0 || !args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a UUID string.")));
        }

        Local<Object> hyp_obj = args.This();

        if(!Hypervisor::HasInstance(hyp_obj)) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a Hypervisor instance")));
        }

        String::Utf8Value uuid(args[0]->ToString());

        Hypervisor *hypervisor = ObjectWrap::Unwrap<Hypervisor>(hyp_obj);

        Domain *domain = new Domain();
        domain->domain_ = virDomainLookupByUUIDString(hypervisor->connection(), (const char *) *uuid);

        if(domain->domain_ == NULL) {
            ThrowException(Error::New(virGetLastError()));
            return Null();
        }

        Local<Object> domain_obj = domain->constructor_template->GetFunction()->NewInstance();

        domain->Wrap(domain_obj);

        return scope.Close(domain_obj);
    }

    void DomainGetIdWorker::Execute() {
        unsigned int id = -1;

        id = virDomainGetID(getDomainPtr());

        if(id == -1u) {
            setVirError(virGetLastError());
            return;
        }

        setVal((int)id);
    }

    NOARGS_WORKER_METHOD(GetId, DomainGetIdWorker)

    void DomainGetInfoWorker::Execute() {
        int ret = -1;

        info_ = (virDomainInfoPtr)malloc(sizeof(virDomainInfo));

        if (info_ == NULL) {
            SetErrorMessage("unable to allocate memory");
            return;
        }

        ret = virDomainGetInfo(getDomainPtr(), info_);

        if(ret == -1) {
            setVirError(virGetLastError());
        }
    }

    void DomainGetInfoWorker::HandleOKCallback() {
        NanScope();

        Local<Object> object = NanNew<Object>();

        object->Set(state_symbol, NanNew<Integer>(info_->state)); //virDomainState
        object->Set(max_memory_symbol, NanNew<Number>(info_->maxMem)); //KBytes
        object->Set(memory_symbol, NanNew<Number>(info_->memory)); //KBytes
        object->Set(vcpus_number_symbol, NanNew<Integer>(info_->nrVirtCpu));
        object->Set(cpu_time_symbol, NanNew<Number>(info_->cpuTime)); //nanoseconds

        free(info_);

        Local<Value> argv[] = { NanNull(), object };

        callback->Call(2, argv);
    }

    NOARGS_WORKER_METHOD(GetInfo, DomainGetInfoWorker)

    void DomainGetNameWorker::Execute() {

      setVal((char*)virDomainGetName(getDomainPtr()));

      if(getVal() == NULL) {
        setVirError(virGetLastError());
      }
    }

    NOARGS_WORKER_METHOD(GetName, DomainGetNameWorker)

    void DomainGetUUIDWorker::Execute() {
      int ret = -1;
      char *uuid = new char[VIR_UUID_STRING_BUFLEN];

      ret = virDomainGetUUIDString(getDomainPtr(), uuid);

      if(ret == -1) {
        setVirError(virGetLastError());
        return;
      }

      setVal(uuid);
    }

    NOARGS_WORKER_METHOD(GetUUID, DomainGetUUIDWorker)

    void DomainGetAutostartWorker::Execute() {
        int ret = -1;
        int autostart_;

        ret = virDomainGetAutostart(getDomainPtr(), &autostart_);

        if(ret == -1) {
          setVirError(virGetLastError());
          return;
        }

        setVal(autostart_ == 0 ? false : true);
    }

    NOARGS_WORKER_METHOD(GetAutostart, DomainGetAutostartWorker)

    void DomainSetAutostartWorker::Execute() {
        int ret = -1;

        ret = virDomainSetAutostart(getDomainPtr(), autostart_);

        if(ret == -1) {
          setVirError(virGetLastError());
          return;
        }

        setVal(autostart_ == 0 ? false : true);
    }

    NAN_METHOD(Domain::SetAutostart) {
        NanScope();

        if(args.Length() != 2 || !args[0]->IsBoolean()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a boolean as first argument")));
        }

        if (!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
        bool autostart = args[0]->IsTrue();

        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainSetAutostartWorker(callback, domain->domain_, autostart ? 1 : 0));

        NanReturnUndefined();
    }

    void DomainGetOsTypeWorker::Execute() {

      setVal(virDomainGetOSType(getDomainPtr()));

      if(getVal() == NULL) {
        setVirError(virGetLastError());
      }
    }

    NOARGS_WORKER_METHOD(GetOsType, DomainGetOsTypeWorker)

    void DomainGetMaxMemoryWorker::Execute() {

      setVal(virDomainGetMaxMemory(getDomainPtr()));

      if(getVal() == 0) {
        setVirError(virGetLastError());
      }
    }

    NOARGS_WORKER_METHOD(GetMaxMemory, DomainGetMaxMemoryWorker)

    void DomainSetMaxMemoryWorker::Execute() {

        int ret = -1;

        ret = virDomainSetMaxMemory(getDomainPtr(), maxmem_);

        if(ret == -1) {
            setVirError(virGetLastError());
        }
    }

    NAN_METHOD(Domain::SetMaxMemory) {
        NanScope();

        if(args.Length() != 2 || !args[0]->IsInt32()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a number as first argument")));
        }

        if (!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainSetMaxMemoryWorker(callback, domain->domain_, args[0]->Int32Value()));

        NanReturnUndefined();
    }

    void DomainSetMemoryWorker::Execute() {

        int ret = -1;

        ret = virDomainSetMemory(getDomainPtr(), mem_);

        if(ret == -1) {
            setVirError(virGetLastError());
        }
    }

    NAN_METHOD(Domain::SetMemory) {
        NanScope();

        if(args.Length() != 2 || !args[0]->IsInt32()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a valid amount of memory")));
        }

        if (!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainSetMemoryWorker(callback, domain->domain_, args[0]->Int32Value()));

        NanReturnUndefined();
    }

    void DomainGetMaxVcpusWorker::Execute() {

        setVal(virDomainGetMaxVcpus(getDomainPtr()));

        if(getVal() == -1) {
            setVirError(virGetLastError());
        }
    }

    NOARGS_WORKER_METHOD(GetMaxVcpus, DomainGetMaxVcpusWorker)

    void DomainIsActiveWorker::Execute() {
        int ret = -1;

        ret = virDomainIsActive(getDomainPtr());

        if(ret == -1) {
            setVirError(virGetLastError());
            return;
        }

        setVal(ret == 1 ? true : false);
    }

    NOARGS_WORKER_METHOD(IsPersistent, DomainIsPersistentWorker)

    void DomainIsPersistentWorker::Execute() {
        int ret = -1;

        ret = virDomainIsPersistent(getDomainPtr());

        if(ret == -1) {
            setVirError(virGetLastError());
            return;
        }

        setVal(ret == 1 ? true : false);
    }

    NOARGS_WORKER_METHOD(IsActive, DomainIsActiveWorker)

    void DomainIsUpdatedWorker::Execute() {
        int ret = -1;

        ret = virDomainIsUpdated(getDomainPtr());

        if(ret == -1) {
            setVirError(virGetLastError());
            return;
        }

        setVal(ret == 1 ? true : false);
    }

    NOARGS_WORKER_METHOD(IsUpdated, DomainIsUpdatedWorker)

    void DomainRebootWorker::Execute() {
        int ret = -1;

        ret = virDomainReboot(getDomainPtr(), 0);

        if(ret == -1) {
            setVirError(virGetLastError());
        }
    }

    NOARGS_WORKER_METHOD(Reboot, DomainRebootWorker)

    void DomainResetWorker::Execute() {
        int ret = -1;

        ret = virDomainReset(getDomainPtr(), 0);

        if(ret == -1) {
            setVirError(virGetLastError());
        }
    }

    NOARGS_WORKER_METHOD(Reset, DomainResetWorker)

    void DomainSaveWorker::Execute() {
        int ret = -1;

        ret = virDomainSave(getDomainPtr(), path_.c_str());

        if(ret == -1) {
            setVirError(virGetLastError());
        }
    }

    NAN_METHOD(Domain::Save) {
        NanScope();

        if(args.Length() != 2 || !args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a string as first argument")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        String::Utf8Value path(args[0]->ToString());

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainSaveWorker(callback, domain->domain_, *path));

        NanReturnUndefined();
    }

    void DomainRestoreWorker::Execute() {
        int ret = -1;

        ret = virDomainRestore(getConnection(), path_.c_str());

        if(ret == -1) {
            setVirError(virGetLastError());
        }
    }

    NAN_METHOD(Domain::Restore) {
        NanScope();

        if(args.Length() != 2 || !args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a string as first argument")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        Local<Object> hyp_obj = args.This();

        if(!Hypervisor::HasInstance(hyp_obj)) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a Hypervisor object instance")));
        }

        String::Utf8Value path(args[0]->ToString());

        Hypervisor *hypervisor = ObjectWrap::Unwrap<Hypervisor>(hyp_obj);

        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainRestoreWorker(callback, hypervisor->connection(), *path));

        NanReturnUndefined();
    }

    void DomainSuspendWorker::Execute() {
        int ret = -1;

        ret = virDomainSuspend(getDomainPtr());

        if(ret == -1) {
            setVirError(virGetLastError());
        }
    }

    NOARGS_WORKER_METHOD(Suspend, DomainSuspendWorker)

    void DomainResumeWorker::Execute() {
        int ret = -1;

        ret = virDomainResume(getDomainPtr());

        if(ret == -1) {
            setVirError(virGetLastError());
        }
    }

    NOARGS_WORKER_METHOD(Resume, DomainResumeWorker)

    void DomainShutdownWorker::Execute() {
        int ret = -1;

        ret = virDomainShutdown(getDomainPtr());

        if(ret == -1) {
            setVirError(virGetLastError());
        }
    }

    NOARGS_WORKER_METHOD(Shutdown, DomainShutdownWorker)

    void DomainStartWorker::Execute() {
        int ret = -1;

        ret = virDomainCreate(getDomainPtr());

        if(ret == -1) {
            setVirError(virGetLastError());
        }
    }

    NOARGS_WORKER_METHOD(Start, DomainStartWorker)

    Handle<Value> Domain::SendKey(const Arguments& args) {
        HandleScope scope;
        int ret = -1;

        if(args.Length() == 0) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify arguments to invoke this function")));
        }

        if(!args[0]->IsArray()) {
            return ThrowException(Exception::TypeError(
            String::New("Argument must be an array of objects")));
        }

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        unsigned int keycodes[VIR_DOMAIN_SEND_KEY_MAX_KEYS];

        Local<Array> keycodes_ = Local<Array>::Cast(args[0]);

        unsigned int length = keycodes_->Length();

        for(unsigned int i = 0; i < length; i++) {
            keycodes[i] = (unsigned int) keycodes_->Get(Integer::New(i))->Int32Value();
        }

        ret = virDomainSendKey(domain->domain_, 0, 150, keycodes, length, 0);

        if(ret == -1) {
            ThrowException(Error::New(virGetLastError()));
            return False();
        }
        return True();
    }

    void DomainGetVcpusWorker::Execute() {
        int ncpus;

        if(virDomainGetInfo(getDomainPtr(), &info_) == -1) {
            setVirError(virGetLastError());
            return;
        }

        if(virNodeGetInfo(getConnection(), &nodeinfo_) == -1) {
            setVirError(virGetLastError());
            return;
        }

        cpuinfo_.resize(info_.nrVirtCpu);
        //memset(cpuinfo_.data(), 0, sizeof(*cpuinfo_) * info_.nrVirtCpu);

        cpumaplen_ = VIR_CPU_MAPLEN(VIR_NODEINFO_MAXCPUS(nodeinfo_));

        cpumap_.resize(info_.nrVirtCpu * cpumaplen_, 0);

        ncpus = virDomainGetVcpus(getDomainPtr(), cpuinfo_.data(), info_.nrVirtCpu, cpumap_.data(), cpumaplen_);

        if(ncpus < 0) {
            setVirError(virGetLastError());
        }
    }

    void DomainGetVcpusWorker::HandleOKCallback() {
        NanScope();

        Local<Array> vcpus = Array::New(info_.nrVirtCpu);

        for(int i = 0; i < info_.nrVirtCpu; i++) {
            Local<Object> obj = NanNew<Object>();
            obj->Set(number_symbol, NanNew<Integer>(cpuinfo_[i].number));
            obj->Set(state_symbol, NanNew<Integer>(cpuinfo_[i].state));
            obj->Set(cpu_time_symbol, NanNew<Number>(cpuinfo_[i].cpuTime)); //nanoseconds
            obj->Set(cpu_symbol, NanNew<Integer>(cpuinfo_[i].cpu));

            int maxcpus =  VIR_NODEINFO_MAXCPUS(nodeinfo_);
            Local<Array> affinity = NanNew<Array>(maxcpus);
            for(int j = 0; j < maxcpus; j++) {
                Local<Object> cpu = NanNew<Object>();
                cpu->Set(cpu_symbol, NanNew<Integer>(j));
                cpu->Set(usable_symbol, NanNew<Boolean>(VIR_CPU_USABLE(cpumap_, cpumaplen_, i, j)));

                affinity->Set(NanNew<Integer>(j), cpu);
            }
            obj->Set(affinity_symbol, affinity);

            vcpus->Set(NanNew<Integer>(i), obj);
        }

        Local<Value> argv[] = { NanNull(), vcpus };

        callback->Call(2, argv);
    }

    NOARGS_WORKER_METHOD(GetVcpus, DomainGetVcpusWorker)

    void DomainSetVcpusWorker::Execute() {
        int ret = -1;

        ret = virDomainSetVcpus(getDomainPtr(), vcpus_);

        if(ret == -1) {
            setVirError(virGetLastError());
        }
    }

    NAN_METHOD(Domain::SetVcpus) {
        NanScope();

        if(args.Length() != 2 || !args[0]->IsInt32()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a number as first argument")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainSetVcpusWorker(callback, domain->domain_, args[0]->Int32Value()));

        NanReturnUndefined();
    }

    void DomainMigrateWorker::Execute() {

        if(conn_) {
            migrated_ = virDomainMigrate(getDomainPtr(),
                conn_,
                flags_,
                destname_.c_str(),
                uri_.c_str(),
                bandwidth_);

            if(migrated_ == NULL) {
                setVirError(virGetLastError());
            }
        } else {
            int ret = -1;

            ret = virDomainMigrateToURI(getDomainPtr(),
                uri_.c_str(),
                flags_,
                destname_.c_str(),
                bandwidth_);

            if(ret == -1) {
                setVirError(virGetLastError());
            }
        }
    }

    void DomainMigrateWorker::HandleOKCallback() {
        NanScope();

        if (migrated_ != NULL) {
            Domain *domain = new Domain();
            domain->domain_ = migrated_;

            Local<Object> domain_obj = domain->constructor_template->GetFunction()->NewInstance();
            domain->Wrap(domain_obj);

            Local<Value> argv[] = { NanNull(), domain_obj };

            callback->Call(2, argv);
        } else {
            callback->Call(0, NULL);
        }
    }

    Handle<Value> Domain::Migrate(const Arguments& args) {
        HandleScope scope;
        unsigned long flags = 0;
        unsigned long bandwidth = 0;

        if(args.Length() != 2 || !args[0]->IsObject()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify an object as first argument")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify an function as second argument")));
        }

        Local<Object> args_ = args[0]->ToObject();

        if(!args_->Has(migration_uri_symbol)) {
            return ThrowException(Exception::TypeError(
            String::New("You must have set property dest_uri in the object")));
        }

        //dest_uri
        String::Utf8Value dest_uri(args_->Get(migration_uri_symbol));

        //dest_name
        //if(args_->Has(migration_name_symbol)) {
            String::Utf8Value dest_name(args_->Get(migration_name_symbol));
            //dest_name = ToCString(dest_name_);
        //}

        //flags
        if(args_->Has(migration_flags_symbol)){
            Local<Array> flags_ = Local<Array>::Cast(args_->Get(migration_flags_symbol));
            unsigned int length = flags_->Length();

            for (unsigned int i = 0; i < length; i++) {
                flags |= flags_->Get(Integer::New(i))->Int32Value();
            }
        }

        //bandwidth (Mbps)
        if(args_->Has(migration_bandwidth_symbol)) {
            bandwidth = args_->Get(migration_bandwidth_symbol)->Int32Value();
        }

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[1].As<Function>());
        DomainMigrateWorker *worker;

        if(args_->Has(migration_hypervisor_symbol)) {
            Local<Object> hyp_obj = args_->Get(migration_hypervisor_symbol)->ToObject();

            if(!Hypervisor::HasInstance(hyp_obj)) {
                return ThrowException(Exception::TypeError(
                String::New("You must specify a Hypervisor object instance")));
            }

            Hypervisor *hypervisor = ObjectWrap::Unwrap<Hypervisor>(hyp_obj);

            worker = new DomainMigrateWorker(callback, domain->domain_, hypervisor->connection());
        } else {
            worker = new DomainMigrateWorker(callback, domain->domain_, *dest_uri);
        }

        worker->setBandwidth(bandwidth);
        worker->setFlags(flags);
        worker->setDestname(*dest_name);

        NanAsyncQueueWorker(worker);

        NanReturnUndefined();
    }

    void DomainSetMigrationMaxDowntimeWorker::Execute() {
        if(virDomainMigrateSetMaxDowntime(getDomainPtr(), downtime_, flags_) == -1) {
            setVirError(virGetLastError());
        }
    }

    NAN_METHOD(Domain::SetMigrationMaxDowntime) {
        NanScope();

        long long downtime = 0;
        unsigned int flags = 0;

        if(args.Length() != 2 || !args[0]->IsInt32()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a number as first argument")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        downtime = args[0]->Int32Value();

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainSetMigrationMaxDowntimeWorker(callback, domain->domain_, downtime, flags));

        NanReturnUndefined();
    }

    DomainPinVcpuWorker::DomainPinVcpuWorker(NanCallback *callback, virDomainPtr domainptr, int vcpu, Handle<Array> cpus)
        : DomainWorker(callback, domainptr), vcpu_(vcpu) {
            NanScope();

            length_ = cpus->Length();
            usables_.resize(length_);
            vcpus_.resize(length_);

            for(int i = 0; i < length_; i++) {

                if(!cpus->Get(NanNew<Integer>(i))->IsObject()) {
                    ThrowException(Exception::TypeError(
                        String::New("The second argument must be an array of objects")));
                }

                Local<Object> cpu = cpus->Get(NanNew<Integer>(i))->ToObject();

                usables_[i] = cpu->Get(usable_symbol)->IsTrue();
                vcpus_[i] = cpu->Get(cpu_symbol)->Int32Value();
            }
    }

    void DomainPinVcpuWorker::Execute() {
        virNodeInfo nodeinfo;
        int maxcpus;
        int cpumaplen;

        if(virNodeGetInfo(getConnection(), &nodeinfo) == -1) {
            setVirError(virGetLastError());
            return;
        }

        maxcpus = VIR_NODEINFO_MAXCPUS(nodeinfo);
        cpumaplen = VIR_CPU_MAPLEN(maxcpus);

        std::vector<unsigned char> cpumap(cpumaplen);

        for(int i = 0; i < length_; i++) {
            if(i > maxcpus) {
                break;
            }

            if(usables_[i]) {
                VIR_USE_CPU(cpumap.data(), vcpus_[i]);
            } else {
                VIR_UNUSE_CPU(cpumap.data(), vcpus_[i]);
            }
        }

        if(virDomainPinVcpu(getDomainPtr(), vcpu_, cpumap.data(), cpumaplen) == -1) {
            setVirError(virGetLastError());
            return;
        }
    }

    NAN_METHOD(Domain::PinVcpu) {
        NanScope();

        if(args.Length() < 2) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify two arguments")));
        }

        if(args.Length() != 3 || !args[0]->IsInt32()) {
            return ThrowException(Exception::TypeError(
            String::New("The first argument must be an integer")));
        }

        if(!args[1]->IsArray()) {
            return ThrowException(Exception::TypeError(
            String::New("The second argument must be an array of objects")));
        }

        if(!args[2]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("The third argument must be a function")));
        }

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        Local<Array> cpus = Local<Array>::Cast(args[1]);

        NanCallback *callback = new NanCallback(args[2].As<Function>());

        NanAsyncQueueWorker(new DomainPinVcpuWorker(callback, domain->domain_, args[0]->Int32Value(), cpus));

        NanReturnUndefined();

    }

    void DomainAttachDeviceWorker::Execute() {
        int ret = -1;

        if(flags_ > 0) {
            ret = virDomainAttachDeviceFlags(getDomainPtr(), xml_.c_str(), flags_);
        } else {
            ret = virDomainAttachDevice(getDomainPtr(), xml_.c_str());
        }

        if (ret == -1) {
            setVirError(virGetLastError());
        }
    }

    NAN_METHOD(Domain::AttachDevice) {
        NanScope();

        unsigned int flags = 0;

        int argsl = args.Length();

        if(argsl != 2 && argsl != 3) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify two or three arguments")));
        }

        if(!args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("The first argument must be an integer")));
        }

        if(argsl == 2 && !args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("The second argument must be an function or an array with flags")));
        }

        if(argsl == 3 && !args[1]->IsArray()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify an array with flags")));
        }

        if(argsl == 3 && !args[2]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("The third argument must be an function")));
        }

        String::Utf8Value xml(args[0]->ToString());

        //flags
        Local<Array> flags_ = Local<Array>::Cast(args[1]);
        unsigned int length = flags_->Length();

        for (unsigned int i = 0; i < length; i++) {
            flags |= flags_->Get(NanNew<Integer>(i))->Int32Value();
        }

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[argsl == 3 ? 2 : 1].As<Function>());

        NanAsyncQueueWorker(new DomainAttachDeviceWorker(callback, domain->domain_, *xml, flags));

        NanReturnUndefined();
    }

    void DomainDetachDeviceWorker::Execute() {
        int ret = -1;

        if(flags_ > 0) {
            ret = virDomainDetachDeviceFlags(getDomainPtr(), xml_.c_str(), flags_);
        } else {
            ret = virDomainDetachDevice(getDomainPtr(), xml_.c_str());
        }

        if (ret == -1) {
            setVirError(virGetLastError());
        }
    }

    NAN_METHOD(Domain::DetachDevice) {
        NanScope();

        unsigned int flags = 0;

        int argsl = args.Length();

        if(argsl != 2 && argsl != 3) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify at two or three arguments")));
        }

        if(!args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("The first argument must be an integer")));
        }

        if(argsl == 2 && !args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("The second argument must be an function or an object with flags")));
        }

        if(argsl == 3 && !args[1]->IsArray()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify an object with flags")));
        }

        if(argsl == 3 && !args[2]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("The third argument must be an function")));
        }

        String::Utf8Value xml(args[0]->ToString());

        //flags
        Local<Array> flags_ = Local<Array>::Cast(args[1]);
        unsigned int length = flags_->Length();

        for (unsigned int i = 0; i < length; i++) {
            flags |= flags_->Get(NanNew<Integer>(i))->Int32Value();
        }

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[argsl == 3 ? 2 : 1].As<Function>());

        NanAsyncQueueWorker(new DomainDetachDeviceWorker(callback, domain->domain_, *xml, flags));

        NanReturnUndefined();
    }

    void DomainUpdateDeviceWorker::Execute() {
        int ret = -1;

        ret = virDomainUpdateDeviceFlags(getDomainPtr(), xml_.c_str(), flags_);

        if (ret == -1) {
            setVirError(virGetLastError());
        }
    }

    NAN_METHOD(Domain::UpdateDevice) {
        NanScope();

        unsigned int flags = 0;

        int argsl = args.Length();

        if(argsl != 3) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify three arguments")));
        }

        if(!args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("The first argument must be an integer")));
        }

        if(!args[1]->IsArray()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify an object with flags")));
        }

        if(!args[2]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("The third argument must be an function")));
        }

        String::Utf8Value xml(args[0]->ToString());

        //flags
        Local<Array> flags_ = Local<Array>::Cast(args[1]);
        unsigned int length = flags_->Length();

        for (unsigned int i = 0; i < length; i++) {
            flags |= flags_->Get(NanNew<Integer>(i))->Int32Value();
        }

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[2].As<Function>());

        NanAsyncQueueWorker(new DomainUpdateDeviceWorker(callback, domain->domain_, *xml, flags));

        NanReturnUndefined();
    }

    Handle<Value> Domain::Destroy(const Arguments& args) {
        HandleScope scope;
        int ret = -1;

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        ret = virDomainDestroy(domain->domain_);

        if(ret == -1) {
            ThrowException(Error::New(virGetLastError()));
            return False();
        }
        return True();
    }

    void DomainToXmlWorker::Execute() {

      setVal(virDomainGetXMLDesc(getDomainPtr(), flags_));

      if(getVal() == NULL) {
        setVirError(virGetLastError());
      }
    }

    NAN_METHOD(Domain::ToXml) {
        NanScope();

        int flags = 0;

        if(args.Length() != 2 || !args[0]->IsArray()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify an array as first argument to invoke this function")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        //flags
        Local<Array> flags_ = Local<Array>::Cast(args[0]);
        unsigned int length = flags_->Length();

        for (unsigned int i = 0; i < length; i++) {
            flags |= flags_->Get(NanNew<Integer>(i))->Int32Value();
        }

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainToXmlWorker(callback, domain->domain_, flags));

        NanReturnUndefined();
    }

    void DomainGetJobInfoWorker::Execute() {
        if(virDomainGetJobInfo(getDomainPtr(), &info_) == -1) {
            setVirError(virGetLastError());
        }
    }

    void DomainGetJobInfoWorker::HandleOKCallback() {
        NanScope();

        Local<Object> info = NanNew<Object>();
        info->Set(type_symbol, NanNew<Integer>(info_.type));

        //time
        Local<Object> time = NanNew<Object>();
        time->Set(elapsed_symbol, NanNew<Number>(info_.timeElapsed));
        time->Set(remaining_symbol, NanNew<Number>(info_.timeRemaining));

        //data
        Local<Object> data = NanNew<Object>();
        data->Set(total_symbol, NanNew<Number>(info_.dataTotal));
        data->Set(processed_symbol, NanNew<Number>(info_.dataProcessed));
        data->Set(remaining_symbol, NanNew<Number>(info_.dataRemaining));

        //memory
        Local<Object> memory = NanNew<Object>();
        memory->Set(total_symbol, NanNew<Number>(info_.memTotal));
        memory->Set(processed_symbol, NanNew<Number>(info_.memProcessed));
        memory->Set(remaining_symbol, NanNew<Number>(info_.memRemaining));

        //file
        Local<Object> file = NanNew<Object>();
        file->Set(total_symbol, NanNew<Number>(info_.fileTotal));
        file->Set(processed_symbol, NanNew<Number>(info_.fileProcessed));
        file->Set(remaining_symbol, NanNew<Number>(info_.fileRemaining));

        info->Set(time_symbol, time);
        info->Set(data_symbol, data);
        info->Set(memory_symbol, memory);
        info->Set(file_symbol, file);

        Local<Value> argv[] = { NanNull(), info };

        callback->Call(2, argv);
    }

    NAN_METHOD(Domain::GetJobInfo) {
        NanScope();

        if(args.Length() != 1 || !args[0]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as first argument to invoke this function")));
        }

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[0].As<Function>());

        NanAsyncQueueWorker(new DomainGetJobInfoWorker(callback, domain->domain_));

        NanReturnUndefined();
    }

    void DomainAbortCurrentJobWorker::Execute() {
        if(virDomainAbortJob(getDomainPtr()) == -1) {
            setVirError(virGetLastError());
        }
    }

    NOARGS_WORKER_METHOD(AbortCurrentJob, DomainAbortCurrentJobWorker)

    NAN_METHOD(Domain::GetSchedType) {
        NanScope();
        NanReturnUndefined();
    }

    void DomainGetSchedParamsWorker::Execute() {
        int nparams = 0;
        char *type = NULL;

        type = virDomainGetSchedulerType(getDomainPtr(), &nparams);
        if (type == NULL) {
            setVirError(virGetLastError());
            return;
        }
        free(type);

        params_.resize(nparams);

        if(virDomainGetSchedulerParameters(getDomainPtr(), params_.data(), &nparams) == -1) {
            setVirError(virGetLastError());
        }
    }

    void DomainGetSchedParamsWorker::HandleOKCallback() {
        NanScope();

        Local<Object> params = NanNew<Object>();

        for (std::vector<virSchedParameter>::iterator it = params_.begin() ; it != params_.end(); ++it) {
            Local<Value> value = Local<Value>::New(Null());

            switch(it->type) {
                case VIR_DOMAIN_SCHED_FIELD_INT:
                    value = NanNew<Integer>(it->value.i);
                    break;
                case VIR_DOMAIN_SCHED_FIELD_UINT:
                    value = NanNew<Integer>(it->value.ui);
                    break;
                case VIR_DOMAIN_SCHED_FIELD_LLONG:
                    value = NanNew<Number>(it->value.l);
                    break;
                case VIR_DOMAIN_SCHED_FIELD_ULLONG:
                    value = NanNew<Number>(it->value.ul);
                    break;
                case VIR_DOMAIN_SCHED_FIELD_DOUBLE:
                    value = NanNew<Number>(it->value.d);
                    break;
                case VIR_DOMAIN_SCHED_FIELD_BOOLEAN:
                    value = NanNew<Integer>(it->value.b);
                    break;
            }

            params->Set(String::New(it->field), value);
        }

        Local<Value> argv[] = { NanNull(), params };

        callback->Call(2, argv);
    }

    NOARGS_WORKER_METHOD(GetSchedParams, DomainGetSchedParamsWorker)

    DomainSetSchedParamsWorker::DomainSetSchedParamsWorker(NanCallback *callback, virDomainPtr domainptr, Handle<Object> newparams)
    : DomainWorker(callback, domainptr), newparams_(newparams->GetOwnPropertyNames()->Length()) {
        for (std::vector<virSchedParameter>::iterator it = newparams_.begin() ; it != newparams_.end(); ++it) {
            //Local<Value> value = newparams->Get(field);
        }
    }

    void DomainSetSchedParamsWorker::Execute() {
        int nparams = 0;
        char *type = NULL;

        type = virDomainGetSchedulerType(getDomainPtr(), &nparams);
        if (type == NULL) {
            setVirError(virGetLastError());
            return;
        }
        free(type);

    }

    /*NAN_METHOD(Domain::SetSchedParams) {
        NanScope();

        if(args.Length() != 2 || !args[0]->IsObject()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify an object as first argument to invoke this function")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify an function as second argument")));
        }

        Local<Object> newparams = args[0]->ToObject();

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
    }*/

    Handle<Value> Domain::SetSchedParams(const Arguments& args) {
        HandleScope scope;
        virSchedParameterPtr params = NULL;
        int nparams = 0;
        char *type = NULL;
        int ret = -1;

        if(args.Length() == 0 || !args[0]->IsObject()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify an object as argument to invoke this function")));
        }

        Local<Object> newparams = args[0]->ToObject();

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
        type = virDomainGetSchedulerType(domain->domain_, &nparams);

        if(type == NULL) {
            ThrowException(Error::New(virGetLastError()));
            return False();
        }
        free(type);

        params = (virSchedParameterPtr) malloc(sizeof(*params) * nparams);
        if(params == NULL) {
            LIBVIRT_THROW_EXCEPTION("unable to allocate memory");
            return False();
        }

        memset(params, 0, sizeof(*params) * nparams);

        ret = virDomainGetSchedulerParameters(domain->domain_, params, &nparams);
        if(ret == -1) {
            ThrowException(Error::New(virGetLastError()));
            free(params);
            return False();
        }

        for(int i = 0; i < nparams; i++) {
            Local<String> field = String::New(params[i].field);
            if(!newparams->Has(field)) {
                continue;
            }

            Local<Value> value = newparams->Get(field);

            switch (params[i].type) {
                case VIR_DOMAIN_SCHED_FIELD_INT:
                    params[i].value.i = value->Int32Value();
                    break;
                case VIR_DOMAIN_SCHED_FIELD_UINT:
                    params[i].value.ui = value->Uint32Value();
                    break;
                case VIR_DOMAIN_SCHED_FIELD_LLONG:
                    params[i].value.l = value->NumberValue();
                    break;
                case VIR_DOMAIN_SCHED_FIELD_ULLONG:
                    params[i].value.ul = value->NumberValue();
                    break;
                case VIR_DOMAIN_SCHED_FIELD_DOUBLE:
                    params[i].value.d = value->NumberValue();
                    break;
                case VIR_DOMAIN_SCHED_FIELD_BOOLEAN:
                    params[i].value.b = value->Uint32Value();
                    break;
            }
        }
        ret = virDomainSetSchedulerParameters(domain->domain_, params, nparams);

        if(ret == -1) {
            ThrowException(Error::New(virGetLastError()));
            free(params);
            return False();
        }
        free(params);

        return True();
    }

    void DomainGetSecurityLabelWorker::Execute() {
        memset(&label_, 0, sizeof label_);

        if(virDomainGetSecurityLabel(getDomainPtr(), &label_) == -1) {
            setVirError(virGetLastError());
        }
    }

    void DomainGetSecurityLabelWorker::HandleOKCallback() {
        NanScope();

        Local<Object> label = NanNew<Object>();
        label->Set(label_symbol, NanNew<String>(label_.label));
        label->Set(enforcing_symbol, NanNew<Boolean>(label_.enforcing));

        Local<Value> argv[] = { NanNull(), label };

        callback->Call(2, argv);
    }

    NOARGS_WORKER_METHOD(GetSecurityLabel, DomainGetSecurityLabelWorker)

    void DomainSaveManagedImageWorker::Execute() {
        unsigned int flags = 0;

        if(virDomainManagedSave(getDomainPtr(), flags) == -1) {
            setVirError(virGetLastError());
        }
    }

    NOARGS_WORKER_METHOD(SaveManagedImage, DomainSaveManagedImageWorker)

    void DomainRemoveManagedImageWorker::Execute() {
        unsigned int flags = 0;

        if(virDomainManagedSaveRemove(getDomainPtr(), flags) == -1) {
            setVirError(virGetLastError());
        }
    }

    NOARGS_WORKER_METHOD(RemoveManagedImage, DomainRemoveManagedImageWorker)

    void DomainHasManagedImageWorker::Execute() {
        unsigned int flags = 0;
        int ret = -1;

        ret = virDomainHasManagedSaveImage(getDomainPtr(), flags);

        if(ret == -1) {
            setVirError(virGetLastError());
            return;
        }

        setVal(ret == 1 ? true : false);
    }

    NOARGS_WORKER_METHOD(HasManagedImage, DomainHasManagedImageWorker)

    void DomainMemoryPeekWorker::Execute() {
        if(virDomainMemoryPeek(getDomainPtr(), start_, size_ , buffer_.data(), flags_) == -1) {
            setVirError(virGetLastError());
        }
    }

    void DomainMemoryPeekWorker::HandleOKCallback() {
        NanScope();

        Local<Object> buffer = NanNewBufferHandle(buffer_.data(), size_);

        Local<Value> argv[] = { NanNull(), buffer };

        callback->Call(2, argv);
    }


    NAN_METHOD(Domain::MemoryPeek) {
        NanScope();

        unsigned int flags = 0;
        unsigned long long start = 0;
        size_t size = 0;

        if(args.Length() < 4) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify four arguments to invoke this function")));
        }

        if(!args[0]->IsNumber() || !args[1]->IsNumber()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a number in the first and second argument")));
        }

        if(!args[2]->IsArray()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify an array in the third argument")));
        }

        if(!args[3]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function in the fourth argument")));
        }

        start = args[0]->NumberValue();
        size = args[1]->NumberValue() * sizeof(char *);

        //flags
        Local<Array> flags_ = Local<Array>::Cast(args[2]);
        unsigned int length = flags_->Length();

        for (unsigned int i = 0; i < length; i++) {
            flags |= flags_->Get(Integer::New(i))->Int32Value();
        }

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[3].As<Function>());

        NanAsyncQueueWorker(new DomainMemoryPeekWorker(callback, domain->domain_, start, size, flags));

        NanReturnUndefined();
    }

    void DomainGetMemoryStatsWorker::Execute() {
        unsigned int nr_stats = 0;
        unsigned int flags = 0;

        nr_stats = virDomainMemoryStats(getDomainPtr(), stats_.data(),
                                    VIR_DOMAIN_MEMORY_STAT_NR, flags);

        if(nr_stats == -1u) {
            setVirError(virGetLastError());
        }
    }

    void DomainGetMemoryStatsWorker::HandleOKCallback() {
        NanScope();

        Local<Object> stats = NanNew<Object>();

        for (std::vector<virDomainMemoryStatStruct>::iterator it = stats_.begin() ; it != stats_.end(); ++it) {
            switch (it->tag) {
                case VIR_DOMAIN_MEMORY_STAT_SWAP_IN:
                    stats->Set(memory_stat_swap_in_symbol, NanNew<Number>(it->val));
                    break;
                case VIR_DOMAIN_MEMORY_STAT_SWAP_OUT:
                    stats->Set(memory_stat_swap_out_symbol, NanNew<Number>(it->val));
                    break;
                case VIR_DOMAIN_MEMORY_STAT_MAJOR_FAULT:
                    stats->Set(memory_stat_major_fault_symbol, NanNew<Number>(it->val));
                    break;
                case VIR_DOMAIN_MEMORY_STAT_MINOR_FAULT:
                    stats->Set(memory_stat_minor_fault_symbol, NanNew<Number>(it->val));
                    break;
                case VIR_DOMAIN_MEMORY_STAT_UNUSED:
                    stats->Set(memory_stat_unused_symbol, NanNew<Number>(it->val));
                    break;
                case VIR_DOMAIN_MEMORY_STAT_AVAILABLE:
                    stats->Set(memory_stat_available_symbol, NanNew<Number>(it->val));
                    break;
            }
        }

        Local<Value> argv[] = { NanNull(), stats };

        callback->Call(2, argv);
    }

    NOARGS_WORKER_METHOD(GetMemoryStats, DomainGetMemoryStatsWorker)

    void DomainBlockPeekWorker::Execute() {
        if(virDomainBlockPeek(getDomainPtr(), path_.c_str(), start_, size_ , buffer_.data(), flags_) == -1) {
            setVirError(virGetLastError());
        }
    }

    void DomainBlockPeekWorker::HandleOKCallback() {
        NanScope();

        Local<Object> buffer = NanNewBufferHandle(buffer_.data(), size_);

        Local<Value> argv[] = { NanNull(), buffer };

        callback->Call(2, argv);
    }

    NAN_METHOD(Domain::BlockPeek) {
        NanScope();

        unsigned int flags = 0;
        unsigned long long start = 0;
        size_t size = 0;

        if(args.Length() < 4) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify five arguments to invoke this function")));
        }

        if(!args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a string in the first and second argument")));
        }

        if(!args[1]->IsNumber() || !args[2]->IsNumber()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a number in the second and third argument")));
        }

        if(!args[3]->IsArray()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify an array in the fourth argument")));
        }

        if(!args[4]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function in the fifth argument")));
        }

        String::Utf8Value path(args[0]->ToString());

        start = args[1]->NumberValue();
        size = args[2]->NumberValue() * sizeof(char *);

        //flags
        Local<Array> flags_ = Local<Array>::Cast(args[3]);
        unsigned int length = flags_->Length();

        for (unsigned int i = 0; i < length; i++) {
            flags |= flags_->Get(Integer::New(i))->Int32Value();
        }

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[4].As<Function>());

        NanAsyncQueueWorker(new DomainBlockPeekWorker(callback, domain->domain_, *path, start, size, flags));

        NanReturnUndefined();
    }

    void DomainGetBlockStatsWorker::Execute() {
        if(virDomainBlockStats(getDomainPtr(), path_.c_str(), &stats_, sizeof(stats_)) == -1) {
            setVirError(virGetLastError());
        }
    }

    void DomainGetBlockStatsWorker::HandleOKCallback() {
        NanScope();

        Local<Object> stats = NanNew<Object>();

        stats->Set(block_stat_rd_req_symbol, NanNew<Number>(stats_.rd_req));
        stats->Set(block_stat_rd_bytes_symbol, NanNew<Number>(stats_.rd_bytes));
        stats->Set(block_stat_wr_req_symbol, NanNew<Number>(stats_.wr_req));
        stats->Set(block_stat_wr_bytes_symbol, NanNew<Number>(stats_.wr_bytes));
        stats->Set(block_stat_errs_symbol, NanNew<Number>(stats_.errs));

        Local<Value> argv[] = { NanNull(), stats };

        callback->Call(2, argv);
    }

    Handle<Value> Domain::GetBlockStats(const Arguments& args) {
        NanScope();

        if(args.Length() != 2) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify two arguments to invoke this function")));
        }

        if(!args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a string as first argument")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }


        String::Utf8Value path(args[0]->ToString());

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainGetBlockStatsWorker(callback, domain->domain_, *path));

        NanReturnUndefined();
    }

    void DomainGetBlockInfoWorker::Execute() {
        unsigned int flags = 0;

        if(virDomainGetBlockInfo(getDomainPtr(), path_.c_str(), &info_, flags) == -1) {
            setVirError(virGetLastError());
        }
    }

    void DomainGetBlockInfoWorker::HandleOKCallback() {
        NanScope();

        Local<Object> info = NanNew<Object>();

        info->Set(block_info_capacity_symbol, NanNew<Number>(info_.capacity));
        info->Set(block_info_allocation_symbol, NanNew<Number>(info_.allocation));
        info->Set(block_info_physical_symbol, NanNew<Number>(info_.physical));

        Local<Value> argv[] = { NanNull(), info };

        callback->Call(2, argv);
    }

    Handle<Value> Domain::GetBlockInfo(const Arguments& args) {
        NanScope();

        if(args.Length() != 2) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify two arguments to invoke this function")));
        }

        if(!args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a string as first argument")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }


        String::Utf8Value path(args[0]->ToString());

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainGetBlockInfoWorker(callback, domain->domain_, *path));

        NanReturnUndefined();
    }

    void DomainCoreDumpWorker::Execute() {
        int flags = 0;

        if(virDomainCoreDump(getDomainPtr(), path_.c_str(), flags) == -1) {
            setVirError(virGetLastError());
        }
    }

    NAN_METHOD(Domain::CoreDump) {
        NanScope();

        if(args.Length() != 2) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify two arguments to invoke this function")));
        }

        if(!args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a string as first argument")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        String::Utf8Value path(args[0]->ToString());

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainCoreDumpWorker(callback, domain->domain_, *path));

        NanReturnUndefined();
    }

    void DomainGetInterfaceStatsWorker::Execute() {
        if(virDomainInterfaceStats(getDomainPtr(), device_.c_str(), &stats_, sizeof(stats_)) == -1) {
            setVirError(virGetLastError());
        }
    }

    void DomainGetInterfaceStatsWorker::HandleOKCallback() {
        NanScope();

        Local<Object> stats = NanNew<Object>();

        stats->Set(nwiface_stat_rx_bytes_symbol, NanNew<Number>(stats_.rx_bytes));
        stats->Set(nwiface_stat_rx_packets_symbol, NanNew<Number>(stats_.rx_packets));
        stats->Set(nwiface_stat_rx_errors_symbol, NanNew<Number>(stats_.rx_errs));
        stats->Set(nwiface_stat_rx_drop_symbol, NanNew<Number>(stats_.rx_drop));
        stats->Set(nwiface_stat_tx_bytes_symbol, NanNew<Number>(stats_.tx_bytes));
        stats->Set(nwiface_stat_tx_packets_symbol, NanNew<Number>(stats_.tx_packets));
        stats->Set(nwiface_stat_tx_errors_symbol, NanNew<Number>(stats_.tx_errs));
        stats->Set(nwiface_stat_tx_drop_symbol, NanNew<Number>(stats_.tx_drop));

        Local<Value> argv[] = { NanNull(), stats };

        callback->Call(2, argv);
    }

    NAN_METHOD(Domain::GetInterfaceStats) {
        NanScope();

        if(args.Length() != 2) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify two arguments to invoke this function")));
        }

        if(!args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a string as first argument")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        String::Utf8Value device(args[0]->ToString());

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainGetInterfaceStatsWorker(callback, domain->domain_, *device));

        NanReturnUndefined();
    }

    void DomainHasCurrentSnapshotWorker::Execute() {
        unsigned int flags = 0;
        int ret = -1;

        ret = virDomainHasCurrentSnapshot(getDomainPtr(), flags);

        if (ret == -1) {
            setVirError(virGetLastError());
            return;
        }

        setVal(ret == 1 ? true : false);
    }

    NOARGS_WORKER_METHOD(HasCurrentSnapshot, DomainHasCurrentSnapshotWorker)

    void DomainRevertToSnapshotWorker::Execute() {
        virDomainSnapshotPtr snapshot = NULL;
        unsigned int flags = 0;
        int ret = -1;

        snapshot = virDomainSnapshotLookupByName(getDomainPtr(), name_.c_str(), flags);
        if(snapshot == NULL) {
            setVirError(virGetLastError());
            return;
        }

        ret = virDomainRevertToSnapshot(snapshot, flags);

        if(ret == -1) {
            setVirError(virGetLastError());
        }

        virDomainSnapshotFree(snapshot);
    }

    NAN_METHOD(Domain::RevertToSnapshot) {
        NanScope();

        if(args.Length() != 2) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify two arguments to invoke this function")));
        }

        if(!args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a string as first argument")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        String::Utf8Value name(args[0]->ToString());

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainRevertToSnapshotWorker(callback, domain->domain_, *name));

        NanReturnUndefined();
    }

    void DomainTakeSnapshotWorker::Execute() {
        virDomainSnapshotPtr snapshot = NULL;
        unsigned int flags = 0;

        if(xml_.length() > 0) {
            snapshot = virDomainSnapshotCreateXML(getDomainPtr(), xml_.c_str(), flags);
        } else {
            snapshot = virDomainSnapshotCurrent(getDomainPtr(), flags);
        }

        if(snapshot == NULL) {
            setVirError(virGetLastError());
            return;
        }

        virDomainSnapshotFree(snapshot);
    }

    NAN_METHOD(Domain::TakeSnapshot) {
        NanScope();

        if(args.Length() != 1 && args.Length() != 2) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify one or two arguments to invoke this function")));
        }

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
        DomainTakeSnapshotWorker *worker;

        if(args.Length() == 2) {
            if(!args[0]->IsString()) {
                return ThrowException(Exception::TypeError(
                String::New("You must specify a string as first argument")));
            }

            if(!args[1]->IsFunction()) {
                return ThrowException(Exception::TypeError(
                String::New("You must specify a function as second argument")));
            }

            String::Utf8Value xml(args[0]->ToString());
            NanCallback *callback = new NanCallback(args[1].As<Function>());
            worker = new DomainTakeSnapshotWorker(callback, domain->domain_, *xml);

        } else {
            if(!args[0]->IsFunction()) {
                return ThrowException(Exception::TypeError(
                String::New("You must specify a function as second argument")));
            }

            NanCallback *callback = new NanCallback(args[0].As<Function>());
            worker = new DomainTakeSnapshotWorker(callback, domain->domain_);
        }

        NanAsyncQueueWorker(worker);

        NanReturnUndefined();
    }

    void DomainGetCurrentSnapshotWorker::Execute() {
        unsigned int flags = 0;
        virDomainSnapshotPtr snapshot = NULL;
        char* xml = NULL;

        snapshot = virDomainSnapshotCurrent(getDomainPtr(), flags);

        if(snapshot == NULL) {
            setVirError(virGetLastError());
            return;
        }

        xml = virDomainSnapshotGetXMLDesc(snapshot, flags);

        if(xml == NULL) {
            virDomainSnapshotFree(snapshot);
            setVirError(virGetLastError());
            return;
        }

        virDomainSnapshotFree(snapshot);
        setVal(xml);
    }

    NOARGS_WORKER_METHOD(GetCurrentSnapshot, DomainGetCurrentSnapshotWorker)

    void DomainDeleteSnapshotWorker::Execute() {
        unsigned int flags = 0;
        virDomainSnapshotPtr snapshot = NULL;

        snapshot = virDomainSnapshotLookupByName(getDomainPtr(), name_.c_str(), flags);

        if(snapshot == NULL) {
            setVirError(virGetLastError());
            return;
        }

        if(virDomainSnapshotDelete(snapshot, flags) == -1) {
            setVirError(virGetLastError());
        }

        virDomainSnapshotFree(snapshot);
    }

    NAN_METHOD(Domain::DeleteSnapshot) {
        NanScope();

        if(args.Length() != 2) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify two arguments to invoke this function")));
        }

        if(!args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a string as first argument")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        String::Utf8Value name(args[0]->ToString());

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainDeleteSnapshotWorker(callback, domain->domain_, *name));

        NanReturnUndefined();
    }

    void DomainLookupSnapshotByNameWorker::Execute() {
        unsigned int flags = 0;
        virDomainSnapshotPtr snapshot = NULL;
        char* xml = NULL;

        snapshot = virDomainSnapshotLookupByName(getDomainPtr(), name_.c_str(), flags);

        if(snapshot == NULL) {
            setVirError(virGetLastError());
            return;
        }

        xml = virDomainSnapshotGetXMLDesc(snapshot, flags);

        if(xml == NULL) {
            virDomainSnapshotFree(snapshot);
            setVirError(virGetLastError());
            return;
        }

        virDomainSnapshotFree(snapshot);
        setVal(xml);
    }

    NAN_METHOD(Domain::LookupSnapshotByName) {
        NanScope();

        if(args.Length() != 2) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify two arguments to invoke this function")));
        }

        if(!args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a string as first argument")));
        }

        if(!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
            String::New("You must specify a function as second argument")));
        }

        String::Utf8Value name(args[0]->ToString());

        Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());

        NanCallback *callback = new NanCallback(args[1].As<Function>());

        NanAsyncQueueWorker(new DomainLookupSnapshotByNameWorker(callback, domain->domain_, *name));

        NanReturnUndefined();
    }

    void DomainGetSnapshotsWorker::Execute() {
        unsigned int flags = 0;
        int num_snapshots = 0;

        num_snapshots = virDomainSnapshotNum(getDomainPtr(), flags);
        if(num_snapshots == -1) {
            setVirError(virGetLastError());
            return;
        }

        std::vector<char*> names(num_snapshots);

        if(virDomainSnapshotListNames(getDomainPtr(), names.data(), num_snapshots, flags) == -1) {
            setVirError(virGetLastError());
            return;
        }

        for (std::vector<char*>::iterator it = names.begin() ; it != names.end(); ++it) {
            virDomainSnapshotPtr snapshot = virDomainSnapshotLookupByName(getDomainPtr(), *it, flags);
            char *xml = virDomainSnapshotGetXMLDesc(snapshot, flags);

            xmls_.push_back(xml);
            virDomainSnapshotFree(snapshot);
            free(xml);
        }
    }

    void DomainGetSnapshotsWorker::HandleOKCallback() {
        NanScope();

        Local<Array> snapshots = Array::New(xmls_.size());
        int i = 0;

        for (std::vector<std::string>::iterator it = xmls_.begin() ; it != xmls_.end(); ++it) {
            snapshots->Set(NanNew<Integer>(i++), NanNew<String>(it->c_str()));
        }

        Local<Value> argv[] = { NanNull(), snapshots };

        callback->Call(2, argv);
    }

    NOARGS_WORKER_METHOD(GetSnapshots, DomainGetSnapshotsWorker)

} //namespace NodeLibvirt
