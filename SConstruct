# -*- coding: utf-8 -*-

#----------------------------------------------------------------------------
#
# Set up the Muhkuh Build System.
#
SConscript('mbs/SConscript')
Import('atEnv')

# Create a build environment for the Cortex-R7 and Cortex-A9 based netX chips.
env_cortexR7 = atEnv.DEFAULT.CreateEnvironment(['gcc-arm-none-eabi-4.9', 'asciidoc'])
env_cortexR7.CreateCompilerEnv('NETX4000_RELAXED', ['arch=armv7', 'thumb'], ['arch=armv7-r', 'thumb'])
env_cortexR7.CreateCompilerEnv('NETX4000_FULL', ['arch=armv7', 'thumb'], ['arch=armv7-r', 'thumb'])

global PROJECT_VERSION

#----------------------------------------------------------------------------
# This is the list of sources. The elements must be separated with whitespace
# (i.e. spaces, tabs, newlines). The amount of whitespace does not matter.
sources = """
	src/main.c
"""

astrIncludePaths = ['src', '#platform/src', '#platform/src/lib', '#targets/version']


#----------------------------------------------------------------------------
#
# Build the netX4000 version.
#
env_netx4000 = atEnv.NETX4000_RELAXED.Clone()
env_netx4000.Append(CPPPATH = astrIncludePaths)
env_netx4000.Replace(LDFILE = 'src/netx4000/netx4000_cr7.ld')
src_netx4000 = env_netx4000.SetBuildPath('targets/netx4000', 'src', sources)
elf_netx4000 = env_netx4000.Elf('targets/netx4000/netx4000.elf', src_netx4000)
txt_netx4000 = env_netx4000.ObjDump('targets/netx4000/netx4000.txt', elf_netx4000, OBJDUMP_FLAGS=['--disassemble', '--source', '--all-headers', '--wide'])
bin_netx4000 = env_netx4000.ObjCopy('targets/netx4000/netx4000.bin', elf_netx4000)
tmp_netx4000 = env_netx4000.GccSymbolTemplate('targets/netx4000/snippet.xml', elf_netx4000, GCCSYMBOLTEMPLATE_TEMPLATE='templates/hboot_snippet.xml', GCCSYMBOLTEMPLATE_BINFILE=bin_netx4000[0])

# Create the snippet from the parameters.
aArtifactGroupReverse = ['org', 'muhkuh', 'hboot', 'sniplib']
atSnippet = {
    'group': '.'.join(aArtifactGroupReverse),
    'artifact': 'apply_mmio_netx4000',
    'version': PROJECT_VERSION,
    'vcs_id': env_netx4000.Version_GetVcsIdLong(),
    'vcs_url': env_netx4000.Version_GetVcsUrl(),
    'license': 'GPL-2.0',
    'author_name': 'Muhkuh team',
    'author_url': 'https://github.com/muhkuh-sys',
    'description': 'Apply the current MMIO configuration.',
    'categories': ['netx4000', 'hardware configuration', 'mmio']
}
strArtifactPath = 'targets/snippets/%s/%s/%s' % ('/'.join(aArtifactGroupReverse), atSnippet['artifact'], PROJECT_VERSION)
snippet_netx4000 = env_netx4000.HBootSnippet('%s/%s-%s.xml' % (strArtifactPath, atSnippet['artifact'], PROJECT_VERSION), tmp_netx4000, PARAMETER=atSnippet)

# Create the POM file.
tPOM = env_netx4000.POMTemplate('%s/%s-%s.pom' % (strArtifactPath, atSnippet['artifact'], PROJECT_VERSION), 'templates/pom.xml', POM_TEMPLATE_GROUP=atSnippet['group'], POM_TEMPLATE_ARTIFACT=atSnippet['artifact'], POM_TEMPLATE_VERSION=atSnippet['version'], POM_TEMPLATE_PACKAGING='xml')
