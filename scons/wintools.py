from SCons.Builder import *
from SCons.Script.SConscript import SConsEnvironment

def TOOL_CINDER_RES_RC(env):
	env.Append(TOOLS = 'CINDER_RES_RC')
	def cinder_res_rc(target, source, env):
		'''Creates a target msvc resource.rc file from a Cinder
		Resource.h include'''
		fin = open(str(source[0]))
		RESOURCES = []
		for line in fin:
			tokens = line.split()
			if tokens:
				if tokens[0] == '#define':
					RESOURCES.append(tokens[1]+'\n')
		fin.close()
		fout = open(str(target[0]), 'w')
		fout.write('#include "' + str(source[0]) + '"\n')
		fout.writelines(RESOURCES)
		fout.close()

	env['BUILDERS']['CinderResRc'] = Builder(action = cinder_res_rc)

