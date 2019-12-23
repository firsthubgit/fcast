import Cocoa
import AppKit
import FlutterMacOS

class MainFlutterWindow: NSWindow {
  override func awakeFromNib() {
    let flutterViewController = FlutterViewController.init()
    var windowFrame = self.frame
    
    windowFrame.size = NSSize(width:400, height:367)
    
    super.styleMask.remove(NSWindow.StyleMask.resizable)
    // super.styleMask.remove(NSWindow.StyleMask.titled)
    
    self.contentViewController = flutterViewController
    self.setFrame(windowFrame, display: true)

    RegisterGeneratedPlugins(registry: flutterViewController)
    
    WecastPlugin.register(with: flutterViewController.registrar(forPlugin: "wecast"))

    super.awakeFromNib()
  }
}
