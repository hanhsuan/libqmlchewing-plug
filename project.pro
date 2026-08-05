TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += qmlchewing
CONFIG(unittest){
    SUBDIRS += tests
}
