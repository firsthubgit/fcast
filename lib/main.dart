import 'dart:io';
import 'dart:math';

import 'package:flutter/foundation.dart'
    show debugDefaultTargetPlatformOverride;
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:menubar/menubar.dart';
import 'package:flutter/cupertino.dart';
import 'package:device_info/device_info.dart';

import 'wecast.dart';
import 'network_check_page.dart';

void main() {
  // See https://github.com/flutter/flutter/wiki/Desktop-shells#target-platform-override
  debugDefaultTargetPlatformOverride = TargetPlatform.fuchsia;

  runApp(new MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Demo',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        primarySwatch: Colors.blue,
        // See https://github.com/flutter/flutter/wiki/Desktop-shells#fonts
        fontFamily: 'Roboto',
      ),
      home: MyHomePage(title: '方正投屏'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  MyHomePage({Key key, this.title}) : super(key: key);

  final String title;

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

const String kPublicKey = "-----BEGIN PUBLIC KEY-----\n"
    "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDFSgFlvSEXuj3KHntguiwtA/g9"
    "FzVjOcRuCiHSr4Vi4cHULaFHFHKfIbDSLTYj8GCocA25c3CGaX2nI32adxXiR+rq"
    "lZwMGb+jet3BS+sFMyOaAJ0DWJiJo4p8aaDiOmVH2eKIDqHB9GKRP2cw4V9o3Kie"
    "JZkFRWQIONBaInx0vwIDAQAB\n"
    "-----END PUBLIC KEY-----";

class _MyHomePageState extends State<MyHomePage>
    with SingleTickerProviderStateMixin {
  bool _hasPermission = false;
  Wecast _wecast;
  // last error string
  int _code = 0;
  String _error;
  CastState _castState = CastState.stateNone;

  final TextEditingController textController = TextEditingController();

  final Map<CastState, String> stateText = <CastState, String>{
    CastState.stateCasting: '停止',
    CastState.stateNone: '开始投屏',
    CastState.statePauseOperating: '开始投屏',
    CastState.statePaused: '开始投屏',
  };

  @override
  void initState() {
    super.initState();

    initPlugin().then((_) {});
  }

  @override
  void dispose() {
    if (_wecast != null) {
      _wecast.shutdown();
    }
    super.dispose();
  }

  Future initPlugin() async {
    Wecast.queryPermission().then((res) {
      setState(() {
        _hasPermission = res;
      });
    });

    // menu
    if (Platform.isMacOS) {
      setApplicationMenu([
        Submenu(label: "工具", children: [
          MenuItem(label: "网络诊断", onClicked: _startNetCheck),
        ])
      ]).then((_) {});
    }

    //
    String name = 'pall';
    if (Platform.isAndroid) {
      name = (await DeviceInfoPlugin().androidInfo).host;
    } else if (Platform.isIOS) {
      name = (await DeviceInfoPlugin().iosInfo).name;
    }

    Wecast.init(
      setting: Setting(
        privateUrl: 'http://10.150.108.105',
        // privateUrl: 'http://117.122.223.243',
        corpId: '1497349707',
        publicKey: kPublicKey,
        nickName: name,
      ),
      errorHandle: (int code, String error) {
        setState(() {
          _code = code;
          _error = error;
        });
      },
      stateChange: (state) {
        print('ui got state: $state');
        setState(() {
          _castState = state;
        });
      },
    ).then((instance) {
      _wecast = instance;
    }).catchError((error, stackTrac) {
      setState(() {
        _code = -1;
        _error = error.toString();
      });
    });
  }

  void _startCast() {
    if (!_hasPermission) {
      // macos，每次构建完毕运行都没有权限，只有第二次运行才有
      Wecast.queryPermission().then((res) {
        setState(() {
          _hasPermission = res;
        });
      });
      return;
    }

    assert(_wecast != null);
    if (_castState != CastState.stateCasting) {
      if (textController.value.text.length == 6)
        _wecast.startCast(textController.value.text).then((_) {
          // castStateChanged 回调并不是那么及时，可以先补救一下
          // setState(() {
          //   _castState = CastState.stateCasting;
          // });
        });
    } else {
      _wecast.stopCast().then((_) {
        textController.text = '';
      });

      // setState(() {
      //   _castState = CastState.stateNone;
      // });
    }
  }

  void _startNetCheck() {
    Navigator.of(context).push(CupertinoPageRoute<void>(
      title: "诊断网络",
      builder: (BuildContext context) => NetworkCheckPage(wecast: _wecast),
    ));
  }

  @override
  Widget build(BuildContext context) {
    final display1 =
        Theme.of(context).textTheme.display1.apply(color: Colors.white70);

    return Scaffold(
      backgroundColor: Colors.blue[900],
      appBar: AppBar(
        centerTitle: true,
        // (${_castState.toString().substring(10)})
        title: Text("方正投屏 - 信息中心"),
        actions: <Widget>[
          IconButton(
            icon: const Icon(Icons.network_check, semanticLabel: '网络诊断'),
            onPressed: _startNetCheck,
          )
        ],
      ),
      body: Center(
        child:
            // !_hasPermission
            // ? Text('检查录屏权限', style: Theme.of(context).textTheme.display1)
            // :
            _wecast == null
                ? Text('正在连接服务器...', style: display1)
                : Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: <Widget>[
                      _hasPermission
                          ? _castState == CastState.stateCasting
                              ? Text('投屏中...', style: display1)
                              : Text('请输入投屏码', style: display1)
                          : Text('没有足够权限', style: display1),
                      SizedBox(height: 20),

                      if (_castState != CastState.stateCasting)
                        Container(
                          width: 300, //
                          child: TextField(
                            autofocus: true,
                            showCursor: true,
                            controller: textController,
                            maxLength: 6,
                            textAlign: TextAlign.center,
                            style: Theme.of(context)
                                .textTheme
                                .display3
                                .copyWith(
                                    color: Colors.white, letterSpacing: 12),
                            decoration: InputDecoration(
                              border: const UnderlineInputBorder(),
                              // hintText: '投屏码(数字)',
                              // helperText: 'Hello',
                              counterText: '',
                              hintStyle: Theme.of(context)
                                  .textTheme
                                  .caption
                                  .apply(fontSizeDelta: 10),
                            ),
                            onSubmitted: (_) {
                              _startCast();
                            },
                            inputFormatters: [
                              WhitelistingTextInputFormatter.digitsOnly,
                            ],
                            keyboardType: TextInputType.number,
                          ),
                        ),

                      SizedBox(height: 40),
                      //
                      RaisedButton(
                        child: Text(
                          stateText[_castState],
                          style: Theme.of(context)
                              .textTheme
                              .title
                              .apply(color: Colors.blue),
                        ),
                        // textColor: ,
                        padding: EdgeInsets.symmetric(horizontal: 50),
                        onPressed: _startCast,
                      ),
                      SizedBox(height: 40),
                      if (_code != 0 && _error != null)
                        SelectableText(
                          _error,
                          style: Theme.of(context)
                              .textTheme
                              .caption
                              .apply(color: Colors.redAccent),
                        ),
                    ],
                  ),
      ),
    );
  }
}
