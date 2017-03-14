#include "manage_devices.h"


int create_master(Display *dpy, char *name){
        //XSync(dpy, False);

        XIAddMasterInfo c;

        c.type = XIAddMaster;
        c.name = name;
        c.send_core = 1;
        c.enable = 1;

        return XIChangeHierarchy(dpy, (XIAnyHierarchyChangeInfo*)&c, 1);
}

XIDeviceInfo * find_device_id(Display *dpy, char *name, int *nbid){
        int nbdev;
        XIDeviceInfo *devlist;
        devlist = XIQueryDevice(dpy,XIAllDevices, &nbdev);
        int i;
        XIDeviceInfo *ids = (XIDeviceInfo*)malloc(sizeof(XIDeviceInfo));
        *nbid=0;
        for(i=nbdev-1; i>=0; i--) {
                //printf("%s\n", devlist[i].name);

                if(strcmp(devlist[i].name,name)==0) {
                        //printf("Device : %s\n",devlist[i].name);
                        ids = (XIDeviceInfo*)realloc(ids, sizeof(XIDeviceInfo)*(*nbid+1));
                        ids[*nbid] = devlist[i];
                        *nbid = *nbid + 1;
                }
        }
        XIFreeDeviceInfo(devlist);
        return ids;
}

int link_devices(Display *dpy, XIDeviceInfo slave, XIDeviceInfo master){
        int ret;
        XIAttachSlaveInfo c;
        //printf(" id slave : %d, id master %d\n",slave.deviceid, master.deviceid);
        c.type = XIAttachSlave;
        c.deviceid = slave.deviceid;
        c.new_master = master.deviceid;
        ret = XIChangeHierarchy(dpy, (XIAnyHierarchyChangeInfo*)&c, 1);
        return ret;
}
