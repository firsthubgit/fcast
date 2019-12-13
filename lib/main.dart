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
      _wecast = await Wecast.init(Setting(
        privateUrl: 'http://117.122.223.243',
        corpId: '1497349707',
        corpAuth: kPublicKey,
        nickName: 'fpall',
      ));
    } catch (error) {
      setState(() {
        _error = error.toString();
      });
    }
  }

  final TextEditingController textController = TextEditingController();

  void _startCast() {
    if (_wecast != null && textController.value.text.length == 6)
      _wecast.startCast(textController.value.text).then((_) {});
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      // appBar: AppBar(title: Text(widget.title)),
      backgroundColor: Colors.blue,
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Text('方正投屏',
                style: Theme.of(context)
                    .textTheme
                    .display1
                    .apply(color: Colors.white)),
            SizedBox(height: 20),
            //
            TextField(
              controller: textController,
              maxLength: 6,
              textAlign: TextAlign.center,
              style: Theme.of(context)
                  .textTheme
                  .display3
                  .apply(color: Colors.white),
              decoration: InputDecoration(
                border: InputBorder.none,
                hintText: '投屏码(数字)',
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
            Divider(color: Colors.white),
            SizedBox(height: 40),
            //
            RaisedButton(
              child: Text('开始投屏',
                  style: Theme.of(context)
                      .textTheme
                      .title
                      .apply(color: Colors.blue)),
              // textColor: ,
              padding: EdgeInsets.symmetric(
                  horizontal: 20, // MediaQuery.of(context).size.width * 0.2,
                  vertical: 20),
              onPressed: _wecast ?? _startCast,
            ),
            Text(_error != null ? _error : ''),
          ],
        ),
      ),
    );
  }
}
