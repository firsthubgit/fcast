import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

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

class _MyHomePageState extends State<MyHomePage> {
  String _platformVersion = 'Unknown';

  @override
  void initState() {
    super.initState();

    initPlugin();
  }

  void initPlugin() async {
    String version;
    try {
      version = await Wecast.platformVersion;
    } catch (error) {
      version = 'Failed to get platform version.';
    }

    setState(() {
      _platformVersion = version;
    });
  }

  final TextEditingController textController = TextEditingController();
  void _startCast() {
    textController.value;
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
                if (_.length == 6) _startCast();
              },
              inputFormatters: [
                WhitelistingTextInputFormatter.digitsOnly,
              ],
            ),
            Divider(color: Colors.white),
            SizedBox(height: 40),
            //
            RaisedButton(
              child: Text('开始投屏 $_platformVersion',
                  style: Theme.of(context)
                      .textTheme
                      .title
                      .apply(color: Colors.blue)),
              // textColor: ,
              padding: EdgeInsets.symmetric(
                  horizontal: 20, // MediaQuery.of(context).size.width * 0.2,
                  vertical: 20),
              onPressed: _startCast,
            ),
            Text(''),
          ],
        ),
      ),
    );
  }
}
