import os
import sys
import json
import plugins
from plugins import *
import inspect

class rasfly_api(object):
	"""docstring for rasfly_api"""
	def __init__(self):
		sys.path.append(os.getcwd() + "/python")
		self.objs = []
		self.keys = ['imu', 'filter', 'controller']
		self.plugin_funcs = {'imu': None, 'filter': None, 'controller': None}
		self.state = {'x': 0, 'y': 0, 'z': 0, 'qw': 0, 'qx': 0, 'qy': 0, 'qz': 0}
		for mod_name in plugins.__all__:
			mod = plugins.__dict__.get(mod_name)
			for (name, obj) in mod.__dict__.items():
				if(inspect.isclass(obj)):
					self.objs.append(self.GetType(name, obj()))

	def GetType(self, name, obj):
		if(obj.name in self.keys):
			self.plugin_funcs[obj.name] = obj.run
			return (name, obj)

		return None

	def BindFunc(self, name):
		return self.plugin_funcs[name]

	def GetState(self):
		self.plugin_funcs['imu'](self.state)
		return json.dumps(self.state)
