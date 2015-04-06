var should = require('should');
var fixture = require('../test/lib/helper').fixture;
var libvirt = require('../build/Release/libvirt');

describe('Domain', function() {
    var hypervisor = new libvirt.Hypervisor('test:///default');

    before(function(done) {
        hypervisor.connect(function(err) {
            if (err) return done(err);
            done();
        });
    });

    it('should create a persistent Domain from its XML Description', function(done) {
        var xml = fixture('domain.xml');

        hypervisor.createDomain(xml, function(err, dom) {
            if (err) return done(err);
            dom.should.be.Object;
            dom.getName(function(err, name) {
                if (err) return done(err);
                name.should.be.equal("nodejs-test");
                done();
            });
        });
    });

    it('should lookup a domain by id', function(done) {
        hypervisor.lookupDomainById(1, function(err, dom) {
            if (err) return done(err);
            dom.should.be.Object;
            done();
        });
    });

    it('should lookup a domain by name', function(done) {
        hypervisor.lookupDomainByName("test", function(err, dom) {
            if (err) return done(err);
            dom.should.be.Object;
            done();
        });
    });

    describe('Methods', function() {
        var domain;

        // Get instance of domain #1
        before(function(done) {
            hypervisor.lookupDomainById(1, function(err, res) {
                if (err) return done(err);
                domain = res;
                done();
            });
        });

        // Start the domain if it has been shutdowned by a test.
        afterEach(function(done) {
            domain.isActive(function(err, res) {
                if (err) return done(err);
                if (!res) {
                    domain.start(function(err) {
                        if (err) return done(err);
                        done();
                    });
                } else {
                    done();
                }
            });
        });

        it('should return the id', function(done) {
            domain.getId(function(err, res) {
                if (err) return done(err);
                res.should.be.equal(1);
                done();
            });
        });

        it('should return the domain information', function(done) {
            domain.getInfo(function(err, res) {
                if (err) return done(err);

                res.state.should.be.equal(domain.VIR_DOMAIN_RUNNING);
                res.max_memory.should.be.equal(8388608);
                res.memory.should.be.equal(2097152);
                res.vcpus_number.should.be.equal(2);
                should.exist(res.cpu_time);
                should.ok(res.cpu_time);
                done();
            });
        });

        it('should return the name', function(done) {
            domain.getName(function(err, res) {
                if (err) return done(err);
                res.should.be.equal("test");
                done();
            });
        });

        it('should return the uuid', function(done) {
            domain.getUUID(function(err, res) {
                if (err) return done(err);
                should.exist(res);
                done();
            });
        });

        it('should indicate if autostart is enable', function(done) {
            domain.getAutostart(function(err, res) {
                if (err) return done(err);
                res.should.be.false;
                done();
            });
        });

        it('should enable or disable autostart', function(done) {
            domain.setAutostart(true, function(err) {
                if (err) return done(err);
                domain.getAutostart(function(err, res) {
                    if (err) return done(err);
                    res.should.be.true;

                    domain.setAutostart(false, function(err) {
                        if (err) return done(err);
                        domain.getAutostart(function(err, res) {
                            res.should.be.false;
                            done();
                        });
                    });
                });
            });
        });

        it('should return the operating system type', function(done) {
            domain.getOsType(function(err, res) {
                if (err) return done(err);
                res.should.be.equal("linux");
                done();
            });
        });

        it('should return maximum amount of physical memory allocated to a domain', function(done) {
            domain.getMaxMemory(function(err, res) {
                if (err) return done(err);
                res.should.be.equal(8388608);
                done();
            });
        });

        it('should change the maximum amount of physical memory allocated to a domain', function(done) {
            domain.setMaxMemory(512000, function(err) {
                if (err) return done(err);
                domain.getMaxMemory(function(err, res) {
                    if (err) return done(err);
                    res.should.be.equal(512000);
                    done();
                });
            });
        });

        it('should dynamically change the runtime amount of memory allocated to a domain', function(done) {
            domain.setMemory(256000, function(err) {
                if (err) return done(err);
                domain.getInfo(function(err, res) {
                    if (err) return done(err);
                    res.memory.should.be.equal(256000);
                    done();
                });
            });
        });

        it('should return the maximum number of virtual CPUs supported for the guest VM', function(done) {
            domain.getMaxVcpus(function(err, res) {
                if (err) return done(err);
                res.should.be.equal(2);
                done();
            });
        });

        it('should indicate whether the domain is active', function(done) {
            domain.isActive(function(err, res) {
                if (err) return done(err);
                res.should.be.true;
                done();
            });
        });

        it('should indicate whether the domain is persistent', function(done) {
            domain.isPersistent(function(err, res) {
                if (err) return done(err);
                res.should.be.true;
                done();
            });
        });

        it('should indicate whether the domain is updated', function(done) {
            domain.isUpdated(function(err, res) {
                if (err) return done(err);
                res.should.be.false;
                done();
            });
        });

        it('should reboot the domain', function(done) {
            domain.reboot(function(err) {
                if (err) return done(err);
                done();
            });
        });

        // Not supported by libvirt test driver
        /*it('should reset the domain', function(done) {
            domain.reset(function(err) {
                if (err) return done(err);
                done();
            });
        });*/

        it('should save and restore the domain', function(done) {
            var path = "/tmp/node-libvirt-saved.img";
            domain.save(path, function(err) {
                if (err) return done(err);
                hypervisor.restoreDomain(path, function(err) {
                    if (err) return done(err);
                    done();
                });
            });
        });

        it('should suspend and resume the domain', function(done) {
            domain.suspend(function(err) {
                if (err) return done(err);
                domain.resume(function(err) {
                    if (err) return done(err);
                    done();
                });
            });
        });

        it('should shutdown the domain', function(done) {
            domain.shutdown(function(err) {
                if (err) return done(err);
                done();
            });
        });


        it('should get information about vcpus', function(done) {
            domain.getVcpus(function(err, res) {
                if (err) return done(err);

                res.should.be.instanceof(Array);
                should.exists(res[0].number);
                should.exists(res[0].state);
                should.exists(res[0].cpu_time);
                should.exists(res[0].cpu);
                should.exists(res[0].affinity);

                var affinity = res[0].affinity;
                var real_cpu = 0; //pedagogical purpose
                should.exists(affinity[real_cpu].usable);

                done();
            });
        });

        it('should dynamically change the number of virtual CPUs used by the domain', function(done) {
            domain.setVcpus(1, function(err) {
                if (err) return done(err);
                done();
            });
        });

        it('should migrate a domain to another hypervisor through a hypervisor connection', function(done) {
            var hypervisor2 = new libvirt.Hypervisor('test:///default');
            var flags = [domain.VIR_MIGRATE_LIVE,
                domain.VIR_MIGRATE_PEER2PEER,
                domain.VIR_MIGRATE_PAUSED,
                domain.VIR_MIGRATE_PERSIST_DEST
            ];

            hypervisor2.connect(function(err) {
                if (err) return done(err);
                domain.migrate({
                    dest_hypervisor: hypervisor2,
                    dest_name: 'test2',
                    dest_uri: '',
                    bandwidth: 100,
                    flags: flags
                }, function(err, domain) {
                    if (err) {
                        err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                    }
                    done();
                });
            });
        });

        it('should migrate a domain to another hypervisor through an uri', function(done) {
            var flags = [domain.VIR_MIGRATE_LIVE,
                domain.VIR_MIGRATE_PEER2PEER,
                domain.VIR_MIGRATE_PAUSED,
                domain.VIR_MIGRATE_PERSIST_DEST
            ];

            domain.migrate({
                dest_name: 'test2',
                dest_uri: 'test:///default',
                bandwidth: 100,
                flags: flags
            }, function(err) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                }
                done();
            });
        });

        it('should set a maximum tolerable time for which the domain is allowed to be paused at the end of live migration', function(done) {
            domain.setMigrationMaxDowntime(100000, function(err) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                }
                done();
            });
        });

        it('should allow to change real CPUs, which can be allocated to a virtual CPU', function(done) {
            domain.getVcpus(function(err, res) {
                if (err) return done(err);

                var affinity = res[0].affinity;
                affinity[0].usable = false;
                affinity[1].usable = false;

                domain.pinVcpu(res[0].number, affinity, function(err) {
                    if (err) return done(err);
                    done();
                });
            });
        });

        it('should attach a device', function(done) {
            var xml = fixture('device.xml');

            //no supported by test driver
            domain.attachDevice(xml, function(err) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                }
                done();
            });
        });

        it('should detach a device', function(done) {
            var xml = fixture('device.xml');

            //no supported by test driver
            domain.detachDevice(xml, function(err) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                }
                done();
            });
        });

        it('should update a device', function(done) {
            var xml = fixture('device_update.xml');
            var flags = [libvirt.VIR_DOMAIN_DEVICE_MODIFY_CONFIG];

            //no supported by test driver
            domain.updateDevice(xml, flags, function(err) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                }
                done();
            });
        });

        it('should return domain xml representation', function(done) {
            var flags = [libvirt.VIR_DOMAIN_XML_SECURE,
                libvirt.VIR_DOMAIN_XML_INACTIVE
            ];

            //no supported by test driver
            domain.toXml(flags, function(err, res) {
                if (err) return done(err);
                res.should.match(/<name>test<\/name>/);
                done();
            });
        });

        it('should extract information about progress of a background job on the domain', function(done) {
            //no supported by test driver
            domain.getJobInfo(function(err, res) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                } else {
                    res.should.be.Object;
                    res.should.have.keys('type', 'time', 'data', 'memory', 'file');
                }
                done();
            });
        });

        it('should abort the current background job on the domain', function(done) {
            //no supported by test driver
            domain.abortCurrentJob(function(err) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                    return done();
                }
                done();
            });
        });

        it('should get the domain scheduler parameters', function(done) {
            //no supported by test driver
            domain.getSchedParams(function(err, res) {
                if (err) return done(err);
                should.exists(res);
                res.should.be.Object;
                res.weight.should.be.equal(50);
                done();
            });
        });

        // TODO setSchedParams

        it('should return the domain security labels', function(done) {
            //no supported by test driver
            domain.getSecurityLabel(function(err, res) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                } else {
                    res.should.be.Object;
                    res.should.have.keys('label', 'enforcing');
                }
                done();
            });
        });

        it('should save a managed image of the domain', function(done) {
            //no supported by test driver
            domain.saveManagedImage(function(err) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                }
                domain.hasManagedImage(function(err, res) {
                    if (err) {
                        err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                    } else {
                        res.should.be.true;
                    }
                    done();
                });
            });
        });

        it('should remove a managed image of the domain', function(done) {
            //no supported by test driver
            domain.removeManagedImage(function(err) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                }
                domain.hasManagedImage(function(err, res) {
                    if (err) {
                        err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                    } else {
                        res.should.be.false;
                    }
                    done();
                });
            });
        });

        it('should allow to read the domain\'s memory content and return it in a Buffer object', function(done) {
            var physical = [domain.VIR_MEMORY_PHYSICAL];
            var virtual = [domain.VIR_MEMORY_VIRTUAL];

            //no supported by test driver
            domain.memoryPeek(0, 1024, physical, function(err, res) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                } else {
                    should.exists(res);
                    res.should.be.instanceof(Buffer);
                }
                domain.memoryPeek(0, 1024, virtual, function(err, res) {
                    if (err) {
                        err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                        return done();
                    } else {
                        should.exists(res);
                        res.should.be.instanceof(Buffer);
                    }
                    return done();
                });
            });
        });

        it('should return domain\'s memory statistics', function(done) {
            //no supported by test driver
            domain.getMemoryStats(function(err, res) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                } else {
                    res.should.be.Object;
                    res.should.have.keys('swap_in', 'swap_out', 'major_fault', 'minor_fault', 'unused', 'available');
                }
                done();
            });
        });

        it('should allow to read the content of a domain\'s disk device and return it in a Buffer object', function(done) {
            //no supported by test driver
            domain.blockPeek('/dev/sda', 0, 1024, [], function(err, res) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                } else {
                    should.exists(res);
                    res.should.be.instanceof(Buffer);
                }
                done();
            });
        });

        it('should return block device stats for block devices attached to the domain', function(done) {
            //no supported by test driver
            domain.getBlockStats('/dev/sda', function(err, res) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_INVALID_ARG);
                } else {
                    res.should.be.Object;
                    res.should.have.keys('read_requests', 'read_bytes', 'write_requests', 'write_bytes');
                }
                done();
            });
        });

        it('should return basic information about a domain\'s block device', function(done) {
            //no supported by test driver
            domain.getBlockInfo('/path', function(err, res) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                } else {
                    res.should.be.Object;
                    res.should.have.keys('capacity', 'allocation', 'physical');
                }
                done();
            });
        });

        it('should dump the core of a domain on a given file for analysis', function(done) {
            //no supported by test driver
            domain.coreDump('/tmp/dumpcore-test.txt', function(err) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                }
                done();
            });
        });

        it('should return network interface statistics of the domain', function(done) {
            //no supported by test driver
            domain.getInterfaceStats('eth1', function(err, res) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_INVALID_ARG);
                } else {
                    res.should.be.Object;
                    res.should.have.keys('rx_bytes', 'rx_packets', 'rx_errors', 'rx_drop', 'tx_bytes', 'tx_packets', 'tx_errors', 'tx_drop');
                }
                done();
            });
        });

        it('should show if the domain has a current snapshot', function(done) {
            domain.hasCurrentSnapshot(function(err, res) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                } else {
                    res.should.be.false;
                }
                done();
            });
        });

        it('should take a snapshot of the domain', function(done) {
            var xml = fixture('snapshot.xml');
            domain.takeSnapshot(xml, function(err) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                }
                done();
            });
        });

        it('should show if the domain has now a current snapshot', function(done) {
            domain.hasCurrentSnapshot(function(err, res) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                } else {
                    res.should.be.true;
                }
                done();
            });
        });

        it('should get the current snapshot', function(done) {
            domain.getCurrentSnapshot(function(err, res) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                } else {
                    res.should.match(/<name>test-snapshot<\/name>/);
                }
                done();
            });
        });

        it('should lookup a snapshot by name', function(done) {
            domain.lookupSnapshotByName('test-snapshot', function(err, res) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                } else {
                    res.should.match(/<name>test-snapshot<\/name>/);
                }
                done();
            });
        });

        it('should revert the domain to a snapshot taken before', function(done) {
            domain.revertToSnapshot('test-snapshot', function(err) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                }
                done();
            });
        });

        it('should return all the domain snapshots', function(done) {
            domain.getSnapshots(function(err, res) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                } else {
                    res.should.be.Array;
                    res.should.have.length(1);
                    res[0].should.match(/<name>test-snapshot<\/name>/);
                }
                done();
            });
        });

        it('should delete a snapshot', function(done) {
            domain.deleteSnapshot('test-snapshot', function(err) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                }
                done();
            });
        });

        it('should show if the domain current snapshot has been deleted', function(done) {
            domain.hasCurrentSnapshot(function(err, res) {
                if (err) {
                    err.code.should.be.equal(err.VIR_ERR_NO_SUPPORT);
                } else {
                    res.should.be.false;
                }
                done();
            });
        });

    });
});
