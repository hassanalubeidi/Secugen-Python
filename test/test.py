import myModule
import time
myModule.init()
myModule.scanFingerprint("f1")
myModule.scanFingerprint("f2")

myModule.matchFingerprints("f1", "f2")