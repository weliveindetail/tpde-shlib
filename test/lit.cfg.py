import os
import lit.formats

config.name = 'TPDE-SHLIB'
config.test_format = lit.formats.ShTest(True)

# Describe test files
config.suffixes = ['.cpp', '.ll']
config.excludes = ["Inputs"]

config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = os.path.join(config.llvm_build_dir, "tools", "tpde-shlib", "test")

config.substitutions.append(('%tpde_shlib', config.tpde_shlib))
config.substitutions.append(('%clang', 'clang++'))

import os
config.environment['PATH'] = os.path.pathsep.join((config.llvm_tools_dir, config.environment.get('PATH', '')))

config.available_features.add('shell')
if hasattr(config, 'enable_plugins') and config.enable_plugins:
    config.available_features.add('plugins')

if config.build_type == "Debug":
    config.available_features.add('llvm-debug')