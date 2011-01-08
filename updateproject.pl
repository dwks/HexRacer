#!/usr/bin/perl
# qmake project update script by DWK

system("qmake -project");

my @project_list = glob "*-*.pro";
if(scalar(@project_list) != 1) {
    die "Error: Project files found != 1: @project_list\n";
}
my $file = $project_list[0];

open(FILE, ">>$file") or die;

print FILE <<EOF;

### Below is automatically added by updateproject.pl

# Necessary compilation settings
INCLUDEPATH += src

# Qt configuration
QT += opengl
OBJECTS_DIR = objects
MOC_DIR = objects

# OpenGL stuff
LIBS += -lGL -lGLU

# Enable debugging mode
CONFIG += debug
EOF

close FILE;

if($file =~ /^(.*)-\d/) {
    rename($file, "$1.pro");
}
