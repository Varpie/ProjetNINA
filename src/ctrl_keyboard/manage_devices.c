#include "manage_devices.h"


int create_master(Display *dpy, char *name){
    XIAddMasterInfo c;

    c.type = XIAddMaster;
    c.name = name;
    c.send_core = 1;
    c.enable = 1;

    return XIChangeHierarchy(dpy, (XIAnyHierarchyChangeInfo*)&c, 1);
}

int* find_device_id(Display *dpy, char *name, int *nbid){
  int nbdev;
  XIDeviceInfo *devlist;
  devlist = XIQueryDevice(dpy,XIAllDevices, &nbdev);
  int i;
  int *ids = (int*)malloc(sizeof(int));
  *nbid=0;
  for(i=nbdev-1;i>=0;i--){
    //printf("%s\n", devlist[i].name);
    if(strcmp(devlist[i].name,name)==0){
      //printf("Device : %s\n",devlist[i].name);
      ids = (int*)realloc(ids, sizeof(int)*(*nbid+1));
      ids[*nbid] = devlist[i].deviceid;
      *nbid = *nbid + 1;
    }
  }
  XIFreeDeviceInfo(devlist);
  return ids;
}

int link_devices(Display *dpy, int id_slave, int id_master){
  return 0;
}
