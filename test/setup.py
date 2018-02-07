from distutils.core import setup, Extension

module = Extension("myModule", sources = ["c_file.c"], language='c++', library_dirs = ['/home/hassan/FDx_SDK_PRO_LINUX3_X64_3_8_0_REV1228/lib/linux3'], libraries = ['sgfplib', 'sgnfiq', 'sgfpamx', 'sgfdu05', 'usb', 'pthread', 'stdc++'])

setup(name="PackageName", version="1.0", description="Yay", ext_modules=[module])