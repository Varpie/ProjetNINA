#include "manage_devices.h"


int create_master(Display *dpy, char *name){
    XIAddMasterInfo c;

    c.type = XIAddMaster;
    c.name = name;
    c.send_core = 1;
    c.enable = 1;

    return XIChangeHierarchy(dpy, (XIAnyHierarchyChangeInfo*)&c, 1);
}
//TODO : Malloc ids
int* find_device_id(Display *dpy, char *name, int *nbid){
  int nbdev;
  XIDeviceInfo *devlist;
  printf("Before XIQueryDevice\n");
  devlist = XIQueryDevice(dpy,XIAllDevices, &nbdev);
  int i;
  int *ids;
  nbid=0;
  for(i=nbdev-1;i>=0;i--){
    printf("%s\n", devlist[i].name);
    if(devlist[i].name == name){
      printf("Device : %s\n",devlist[i].name);
      ids[*nbid] = devlist[i].deviceid;
      nbid++;
    }
  }
  XIFreeDeviceInfo(devlist);
  return ids;
}
