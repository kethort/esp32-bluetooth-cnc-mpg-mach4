import wx
import os
import psutil
import shutil
import zipfile
import xml.etree.ElementTree as et
from subprocess import Popen, PIPE

class AppWindow(wx.Frame):
	dir_changed = False

	def __init__(self):
		wx.Frame.__init__(self, None, id = wx.ID_ANY, title = 'Wireless MPG Installer', pos = wx.DefaultPosition, size = wx.Size(500,233), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL)

		self.SetSizeHints(wx.DefaultSize, wx.DefaultSize)

		fgSizer1 = wx.FlexGridSizer(5, 1, 0, 0)
		fgSizer1.SetFlexibleDirection(wx.BOTH)
		fgSizer1.SetNonFlexibleGrowMode(wx.FLEX_GROWMODE_SPECIFIED)

		bSizer4 = wx.BoxSizer(wx.VERTICAL)

		self.m_staticText3 = wx.StaticText(self, wx.ID_ANY, 'Bluetooth MPG Installer', wx.DefaultPosition, wx.Size(500,60), wx.ALIGN_CENTER_HORIZONTAL)
		self.m_staticText3.Wrap(-1)

		self.m_staticText3.SetFont(wx.Font(30, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL, False, wx.EmptyString))

		bSizer4.Add(self.m_staticText3, 0, wx.ALL, 5)

		fgSizer1.Add(bSizer4, 1, wx.EXPAND, 5)

		bSizer1 = wx.BoxSizer(wx.HORIZONTAL)

		self.m_staticText1 = wx.StaticText(self, wx.ID_ANY, 'Mach4 Directory:', wx.DefaultPosition, wx.DefaultSize, 0)
		self.m_staticText1.Wrap(-1)

		bSizer1.Add(self.m_staticText1, 0, wx.ALIGN_CENTER|wx.ALL, 5)

		self.mach_dir_url = wx.TextCtrl(self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.Size(300,-1), wx.TE_READONLY)
		bSizer1.Add(self.mach_dir_url, 0, wx.ALL, 5)

		self.btn_browse = wx.Button(self, wx.ID_ANY, 'Browse', wx.DefaultPosition, wx.DefaultSize, 0)
		bSizer1.Add(self.btn_browse, 0, wx.ALL, 5)

		fgSizer1.Add(bSizer1, 1, wx.EXPAND, 5)

		bSizer2 = wx.BoxSizer(wx.VERTICAL)

		bSizer2.Add((0, 10), 1, wx.EXPAND, 5)

		fgSizer1.Add(bSizer2, 1, wx.EXPAND, 5)

		bSizer3 = wx.BoxSizer(wx.HORIZONTAL)

		self.m_staticText2 = wx.StaticText(self, wx.ID_ANY, 'Select Profile:      ', wx.DefaultPosition, wx.DefaultSize, 0)
		self.m_staticText2.Wrap(-1)

		bSizer3.Add(self.m_staticText2, 0, wx.ALIGN_CENTER|wx.ALL, 5)

		self.cbo_profile_selection = wx.ComboBox(self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.Size(180,-1), [], wx.CB_READONLY)
		bSizer3.Add(self.cbo_profile_selection, 0, wx.ALL, 5)

		bSizer3.Add((100, 0), 1, wx.EXPAND, 5)

		fgSizer1.Add(bSizer3, 1, wx.EXPAND, 5)
		
		bSizer5 = wx.BoxSizer(wx.HORIZONTAL)

		bSizer5.Add((0, 0), 1, wx.EXPAND, 5)
		self.btn_install = wx.Button(self, wx.ID_ANY, 'Install', wx.DefaultPosition, wx.DefaultSize, 0)
		bSizer5.Add(self.btn_install, 0, wx.ALL, 5)

		fgSizer1.Add(bSizer5, 1, wx.EXPAND, 5)

		self.SetSizer(fgSizer1)
		self.Layout()

		self.Centre(wx.BOTH)

		# event binders
		self.btn_browse.Bind(wx.EVT_BUTTON, self.btn_browse_on_click)
		self.btn_install.Bind(wx.EVT_BUTTON, self.btn_install_on_click)
		self.cbo_profile_selection.Bind(wx.EVT_UPDATE_UI, self.update_profile_choices)

		self.Show()
		self.find_default_mach_installation()

	def find_default_mach_installation(self):
		root_dir = os.path.abspath(os.sep)
		for folder in os.listdir(root_dir):
			if 'Mach4' in folder:
				self.mach_dir_url.ChangeValue(os.path.join(root_dir, folder))
				self.dir_changed = True
				break

	def update_profile_choices(self, event):
		if self.dir_changed:
			self.dir_changed = False
			mach_dir = self.mach_dir_url.GetValue()
			profile_dir = os.path.join(mach_dir, 'Profiles')
			
			profile_choices = ['']
			for profile in os.listdir(profile_dir):
				profile_choices.append(profile)

			self.cbo_profile_selection.Clear()
			self.cbo_profile_selection.Append(profile_choices)
			self.cbo_profile_selection.SetSelection(0)
		event.Skip()

	def btn_browse_on_click(self, event):
		dlg = wx.DirDialog(None, "Choose Mach4 installation directory", "", wx.DD_DEFAULT_STYLE|wx.DD_DIR_MUST_EXIST)
		res = dlg.ShowModal()
		if res == wx.ID_OK:
			file_path = dlg.GetPath()
			self.mach_dir_url.ChangeValue(file_path)
			self.dir_changed = True
		event.Skip()

	def dict_to_machine_ini(self, ini_dict, out_loc):
		with open(out_loc, 'w') as out_file:
			for item in ini_dict:
				out_file.write('[{0}]\n'.format(item))
				properties_dict = ini_dict[item]

				if properties_dict:
					for prop in properties_dict: 
						out_file.write('{0}={1}\n'.format(prop, properties_dict[prop]))
	
	def machine_ini_to_dict(self, ini_loc):
		main_dict = {}
		nested_dict = {}
		heading = ''
		last_line = ''

		with open(ini_loc, 'r') as ini_file:
			for line in ini_file:
				if not line.strip():
					continue

				if '[' in last_line and not '=' in last_line:
					main_dict[heading] = None
					
				if '[' in line and not '=' in line:			
					if nested_dict: 
						main_dict[heading] = nested_dict
						nested_dict = {}
					heading = line.strip('[]\r\n')
				else:
					line = line.rstrip('\r\n').split('=')
					sub_heading = line[0]
					value = line[1]
					nested_dict[sub_heading] = value

				last_line = line

		if nested_dict: 
			main_dict[heading] = nested_dict
			nested_dict = {}

		return main_dict

	def mach_is_running(self):
		# check if Mach4 is running
		mach_running = False

		for process in psutil.process_iter():
			if 'Mach4' in process.name():
				mach_running = True
				wx.MessageBox("Mach4 is currently running. \nPlease close Mach4 and try again.", "Confirm", wx.OK)

		return mach_running

	def backup_machine_ini(self, ini_path, backup_path):
		# make a backup of Machine.ini
		if self.cbo_profile_selection.GetSelection() != 0:			
			try:
				shutil.copy(ini_path, backup_path)
			except Exception as e:
				print('failed to copy file \n {0}'.format(e))

	def enable_plugin_in_ini(self, ini_dict, plugin_name):
		test_val = ini_dict.get('Plugins', None)
		if test_val is None:
			ini_dict['Plugins'] = {}
		ini_dict['Plugins'][plugin_name] = 1

	def add_keyboard_keys_to_plugin(self, ini_dict):
		next_key = self.get_next_available_keyboard_key(ini_dict)
		keys_dict = self.machine_ini_to_dict('keyboard_keys')

		if next_key > 0:
			for i in range(0, len(keys_dict)):
				keys_dict['KeyboardKey' + str(i + next_key)] = keys_dict['KeyboardKey' + str(i)]
				del keys_dict['KeyboardKey' + str(i)]

		ini_dict.update(keys_dict)

		
	def get_next_available_keyboard_key(self, ini_dict):
		next_key = -1

		for key in ini_dict:
			if 'KeyboardKey' in key:
				num = ''.join(i for i in key if i.isdigit())
				if int(num) > int(next_key):
					next_key = int(num)
		
		return next_key + 1

	def btn_install_on_click(self, event):
		mach_dir = self.mach_dir_url.GetValue()
		ini_path = backup_path = ''
		profile = self.cbo_profile_selection.GetValue()
		ini_path = os.path.join(mach_dir, 'Profiles\\{0}\\Machine.ini'.format(profile))
		backup_path = os.path.join(mach_dir, 'Profiles\\{0}\\Backups\\Machine.ini.bak'.format(profile))
		
		if self.mach_is_running():
			return

		self.backup_machine_ini(ini_path, backup_path)

		ini_dict = self.machine_ini_to_dict(ini_path)
		 
		#  enable keyboard and regfile plugins
		self.enable_plugin_in_ini(ini_dict, 'mcRegfile')
		self.enable_plugin_in_ini(ini_dict, 'mcKeyboard')

		# update ini_dict with input signal settings from file
		input_sigs_dict = self.machine_ini_to_dict('input_signals')
		ini_dict.update(input_sigs_dict)

		# update ini_dict with keyboard plugin key mappings
		self.add_keyboard_keys_to_plugin(ini_dict)

		# enable MPG 11 in Machine.ini activate the mousewheel and modify settings
		ini_dict['Mpg11']['Enabled'] = 1 
		ini_dict['Mpg11']['CountsPerDetent'] = 4
		ini_dict['Mpg11']['AccelPercent'] = 100
		ini_dict['Mpg11']['RatePercent'] = 100
		ini_dict['Mpg11']['RegisterPath'] = 'Keyboard/MouseWheel'

		# edit Mach4 screen to update the SigLib input handlers
		screens_dir = os.path.join(mach_dir, 'Screens')
		screen_name = ini_dict['Preferences']['Screen'].strip()
		screen_path = os.path.join(screens_dir, screen_name)
		
		temp_dir = os.path.join(os.getcwd(), 'temp')
		if os.path.exists(temp_dir):
			shutil.rmtree(temp_dir)
		
		os.mkdir(temp_dir)

		# extract screen files from .set file
		with zipfile.ZipFile(screen_path, 'r') as zip_ref:
			zip_ref.extractall(temp_dir)

		# edit the signal library in the Mach4 screen load script
		sig_lib = open('update_sig_lib')
		screen_load = '\n' + sig_lib.read()
		sig_lib.close()

		tree = et.parse(os.path.join(temp_dir, 'screen.xml'))
		
		for node in tree.iter('Event'):    
			if node.attrib['name'] == 'Screen Load Script':
				if not 'update_sig_lib' in node.text:
					node.text += screen_load	

		tree.write(os.path.join(temp_dir, 'screen.xml'))
		
		# re-zip the screen .set file and move it into Mach4 Screens directory
		shutil.make_archive(screen_name, 'zip', temp_dir)
		os.rename(screen_name + '.zip', screen_name)
		shutil.move(screen_name, screen_path)
		shutil.rmtree(temp_dir)

		# copy MPGWiz to wizards directory
		wizards_dir = os.path.join(mach_dir, 'Wizards')
		shutil.copy('MPGWizBluetooth.mcs', wizards_dir)

		# copy keyboard plugins to Mach directory
		plugins_dir = os.path.join(mach_dir, 'Plugins')
		shutil.copy('mcKeyboard.m4pw', plugins_dir)
		shutil.copy('mcKeyboard.sig', plugins_dir)

		# write the edited Machine.ini into the Mach4 Profiles directory
		self.dict_to_machine_ini(ini_dict, ini_path)
		
		wx.MessageBox("Installation complete!", "Confirm", wx.OK)

		event.Skip()

	def __del__(self):
		pass

app = wx.App()
frame = AppWindow()
app.MainLoop()