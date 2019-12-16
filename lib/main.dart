import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:menubar/menubar.dart';

import 'wecast.dart';

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Demo',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        primarySwatch: Colors.blue,
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

class _MyHomePageState extends State<MyHomePage> {
  Wecast _wecast;
  // last error string
  String _error;

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

    initPlugin();
  }

  void initPlugin() async {
    await setApplicationMenu([
      Submenu(label: "工具", children: [
        MenuItem(
            label: "网络检测",
            onClicked: () async {
              if (_wecast != null) {
                await _wecast.startNetCheck();
              }
            }),
      ])
    ]);

    try {
      _wecast = await Wecast.init(
          Setting(
            privateUrl: 'http://117.122.223.243',
            corpId: '1497349707',
            corpAuth: kPublicKey,
            nickName: 'fpall', // TODO: mac/ip or hostname
          ), (String error) {
        if (mounted)
          setState(() {
            _error = error;
          });
      }, (state) {
        setState(() {});
      });
    } catch (error) {
      if (mounted)
        setState(() {
          _error = error.toString();
        });
    }
  }

  void _startCast() {
    if (_wecast != null && textController.value.text.length == 6)
      _wecast.startCast(textController.value.text).then((_) {});
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.blue[900],
      body: Center(
        child: _wecast == null
            ? Text('正在连接服务器...', style: Theme.of(context).textTheme.display1)
            : Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: <Widget>[
                  Text('方正投屏',
                      style: Theme.of(context)
                          .textTheme
                          .display1
                          .apply(color: Colors.white70)),
                  SizedBox(height: 20),
                  //
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
                          .copyWith(color: Colors.white, letterSpacing: 12),
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
                    ),
                  ),

                  SizedBox(height: 40),
                  //
                  RaisedButton(
                    child: Text(
                      stateText[_wecast.state],
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
                  if (_error != null)
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
