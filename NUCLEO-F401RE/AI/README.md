# X-CUBE-AI ver 3.3.0 evaluation

## X-CUBE-AI guide

I just followed the instructions on [this video (YoueTube)](https://www.youtube.com/watch?v=grgNXdkmzzQ&list=PLnMKNibPkDnG9IC5Nl9vJg1CKMAO1kODW&t=141s&index=2).

## Loading Keras model into CubeMX

I loaded a Keras model in (this folder)[../Thermography/tensorflow] to CubeMX.

## Tips for TrueSTUDIO

It seems to require extra steps for building the auto-generated code on TrueSTUDIO.

[Step 1] Rename the auto generated lib file from "network_runtime.a" to "libnetwork_runtime.a" as a shared C library.

![](./truestudio_rename_lib.jpg)

[Step 2] Add the lib path

![](./truestudio_lib_path.jpg)

![](./truestudio_lib_path_edit.jpg)

[Step 3] Add the lib name

![](./truestudio_lib_name.jpg)
