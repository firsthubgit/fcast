import 'package:flutter/material.dart';

import 'wecast.dart';

class NetworkCheckPage extends StatefulWidget {
  NetworkCheckPage({Key key, this.wecast}) : super(key: key);

  Wecast wecast;

  @override
  _NetworkCheckPageState createState() => _NetworkCheckPageState();
}

class _NetworkCheckPageState extends State<NetworkCheckPage> {
  List<String> items = <String>[];
  int progress = 0;
  int total = 6;

  @override
  void initState() {
    super.initState();

    widget.wecast.startNetCheck((String method, arguments) {
      if (method == "netCheck") {
        if (arguments is Map) {
          if (mounted) {
            setState(() {
              progress = arguments['progress'];
              total = arguments['total'];
              items.add(arguments['description']);
            });
          } else {
            progress = arguments['progress'];
            total = arguments['total'];
            items.add(arguments['description']);
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
              title: Text(
                items[i],
                style: Theme.of(context)
                    .textTheme
                    .body1
                    .apply(color: Colors.white),
              ),
              trailing: Icon(Icons.check, color: Colors.green),
            );
          }),
    );
  }
}
