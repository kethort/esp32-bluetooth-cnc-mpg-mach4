from distutils.core import setup
import py2exe
import os

installer_script = os.path.join(os.getcwd(), 'src\\installer\\MPGInstaller.py')
wizard_script = os.path.join(os.getcwd(), 'src\\Mach4Wizard\\MPGWizBluetooth.mcs')
input_sigs_ini = os.path.join(os.getcwd(), 'src\\ini_settings\\input_signals')
keyboard_keys_ini = os.path.join(os.getcwd(), 'src\\ini_settings\\keyboard_keys')
update_screenload = os.path.join(os.getcwd(), 'src\\ini_settings\\update_screenload')
update_plc = os.path.join(os.getcwd(), 'src\\ini_settings\\update_plc')
plugin1 = os.path.join(os.getcwd(), 'src\\Plugins\\mcKeyboard.m4pw')
plugin2 = os.path.join(os.getcwd(), 'src\\Plugins\\mcKeyboard.sig')

data_files = [('', [wizard_script, input_sigs_ini, keyboard_keys_ini, update_screenload, update_plc, plugin1, plugin2])]
setup(windows=[installer_script], data_files = data_files)