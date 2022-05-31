'''
  INSTRUCTIONS:
   ~$ python (or python3) compile_chfem_gpu.py -h (or --help)
'''
#---------------------------------------------------------------------------
import sys
import os
import subprocess as terminal
import shutil
from glob import glob

# default executable name
executable_name = 'dipolar_sum_core.so'
#---------------------------------------------------------------------------
def handle_error(cmd):
  print('Command \"{}\" failed.'.format(cmd))
  print('Failed to compile {}.'.format(executable_name))
  exit()
#--------------------------------------------------------------------------- 
def printVersion():
  print('LCC - UFF')
  print('Python script to compile dipolar_sum cpp lib. Version 1.0 (2022)')
#---------------------------------------------------------------------------
def printHelp():
  printVersion()
  print('----------------------------------------')
  print('~$ python3 compile_dipolar_sum_cpp_lib.py [options]')
  print('----------------------------------------')
  print('\t(-h or --help)          : print this info and exit.')
  print('\t(-v or --version)       : print version header.')
  print('\t(-n or --name) <string> : provide executable name.')
#---------------------------------------------------------------------------
def checkInput():

  # check if user asked for help
  if '-h' in sys.argv or '--help' in sys.argv:
    printHelp()
    exit()
  
  # check if user asked for version
  if '-v' in sys.argv or '--version' in sys.argv:
    printVersion()
    exit()

  # check if user provided a name for executable
  exe_name = None
  input_list = sys.argv
  if '-n' in sys.argv or '--name' in sys.argv:
    try:
      flag_id = input_list.index('-n')
    except ValueError:
      flag_id = input_list.index('--name')
    flag = input_list.pop(flag_id)
    if (flag_id >= len(input_list)):
      print('ERROR: flag \"{}\" must be followed by a string with a name for the executable.'.format(flag))
      print('Aborted before compiling.')
      exit()
    exe_name = input_list.pop(flag_id)

  # append the rest of the input strings as one long string
  string = ''
  for i in range(1,len(input_list)):
    string += ' '+input_list[i]
  
  return string, exe_name
#---------------------------------------------------------------------------
def extractFilename(filepath):
  _, filename = os.path.split(filepath)
  return os.path.splitext(filename)[0]
#---------------------------------------------------------------------------

#-------------------------
##########################
########## main ##########
##########################
#-------------------------

if __name__ == '__main__':

  # check input
  input_string, exe_string = checkInput();
  if not (exe_string is None):
    executable_name = exe_string
      
  # set strings with compilable source code dir and includes 
  # also pre set cpp compiler options
  src_dir = os.path.join(os.getcwd(), os.path.dirname(__file__), '..', 'src')
  include_path = os.path.join(os.getcwd(), os.path.dirname(__file__), '..', 'src', 'include')
  include_cuda_path = os.path.join(os.getcwd(), os.path.dirname(__file__), '..', 'src', 'cpp/cuda')
  include_string = f'-I {include_path}:{include_cuda_path}'
  cpp_opts = f"--compiler-options –x cu"


  # get names of files with .cu and .c extensions and store
  # in list with .o extension (will be used for linking)
  o_files  = [extractFilename(f)+'.o' for f in glob(f"{src_dir}/**/*.cu",recursive=True)]
  o_files += [extractFilename(f)+'.o' for f in glob(f"{src_dir}/**/*.cpp",recursive=True)]
  o_files += [extractFilename(f)+'.o' for f in glob(f"{src_dir}/**/*.c", recursive=True)]
  
  # generate one string containing all elements in list
  o_files_string = ''
  for f in o_files:
    o_files_string += ' '+f

  # array of compiler directives
  compiler_directives = [
    f"nvcc -c ../src/cpp/*.cu {input_string} --compiler-options '-fPIC'",
    f"nvcc -c ../src/cpp/*.cpp {input_string} -x cu --compiler-options '-fPIC'",
    f"nvcc -o {executable_name} --shared {o_files_string} -Xcompiler -fopenmp -O3 --compiler-options '-fPIC' {input_string}"
  ]

  # run compiler commands in terminal
  print('Starting compilation of shared lib')
  for command in compiler_directives:
    print(command)
    if (terminal.call(command,shell=True)):
      handle_error(command)
  
  terminal.call('rm {}'.format(o_files_string),shell=True)
  shutil.move('{}'.format(executable_name), '../src/bin/{}'.format(executable_name))
  
  # print feedback that compiling went ok  
  print('{} binary executable successfully compiled.'.format(executable_name))
 
 
