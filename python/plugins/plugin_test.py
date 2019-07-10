import rasfly
import math

class imu(object):
	"""docstring for base"""
	def __init__(self):
		self.name = 'imu'
	
	def run(self):
		print("imu")

class filter(object):
	"""docstring for filter"""
	def __init__(self):
		self.name = 'filter'

	def run(self, state):
		x, y, z = rasfly.get_rotation()
		rasfly.set_rotation(x+math.radians(5), y, z)
		