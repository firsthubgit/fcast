import 'package:flutter/material.dart';

import 'wecast.dart';

class NetworkCheckPage extends StatefulWidget {
  NetworkCheckPage({Key key, this.wecast}) : super(key: key);

  final Wecast wecast;

  @override
  _NetworkCheckPageState createState() => _NetworkCheckPageState();
}

class LogItem {
  LogItem(this.description, this.url);
  final String description;
  final String url;
}

class _NetworkCheckPageState extends State<NetworkCheckPage> {
  List items = <LogItem>[];
  int progress = 0;
  int total = 6;

  @override
  void initState() {
    super.initState();

    widget.wecast.startNetCheck((String method, arguments) {
      if (method == "netCheck") {
        if (arguments is Map) {
          progress = arguments['progress'];
          total = arguments['total'];
          items.add(LogItem(arguments['description'], arguments['url']));

          if (mounted) {
            setState(() {});
          }
        }
      }
    }).then((_) {});
  }

  @override
  void dispose() {
    if (progress != total) {
      widget.wecast.stopNetCheck().then((_) {});
    }
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.blue[900],
      appBar: AppBar(
        centerTitle: true,
        title: Text("诊断网络"),
        actions: <Widget>[
          Center(
            child: Text("$progress/$total "),
          ),
        ],
      ),
      body: ListView.builder(
          itemCount: items.length,
          itemBuilder: (BuildContext context, int i) {
            return ListTile(
              dense: true,
              title: Text(items[i].description,
                  style: Theme.of(context)
                      .textTheme
                      .body1
                      .apply(color: Colors.white)),
              subtitle: Text(items[i].url,
                  style: Theme.of(context)
                      .textTheme
                      .caption
                      .apply(color: Colors.grey)),
              trailing: Icon(Icons.check, color: Colors.green),
            );
          }),
    );
  }
}
