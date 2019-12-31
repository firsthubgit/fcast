import 'package:flutter/material.dart';

import 'wecast.dart';

class NetworkCheckPage extends StatefulWidget {
  NetworkCheckPage({Key key, this.wecast}) : super(key: key);

  final Wecast wecast;

  @override
  _NetworkCheckPageState createState() => _NetworkCheckPageState();
}

class LogItem {
  LogItem({this.description, this.url, this.success, this.total});
  final String description;
  final String url;
  final int success;
  final int total;
}

class _NetworkCheckPageState extends State<NetworkCheckPage> {
  List<LogItem> items = <LogItem>[];
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
          items.add(
            LogItem(
              description: arguments['description'],
              url: arguments['url'],
            ),
          );

          if (mounted) {
            setState(() {});
          }
        }
      } else if (method == "netChecked") {
        print('netChecked: $arguments');
        if (arguments is List) {
          setState(() {
            for (int i = 0; i < arguments.length; ++i) {
              items[i] = LogItem(
                description: arguments[i]['description'],
                url: arguments[i]['url'],
                total: arguments[i]['total'],
                success: arguments[i]['success'],
              );
            }
          });
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
      body: Scrollbar(
        child: ListView.builder(
            itemCount: items.length,
            itemBuilder: (BuildContext context, int i) {
              final LogItem item = items[i];
              return ListTile(
                dense: true,
                title: Text(item.description,
                    style: Theme.of(context)
                        .textTheme
                        .body1
                        .apply(color: Colors.white)),
                subtitle: Text(item.url,
                    style: Theme.of(context)
                        .textTheme
                        .caption
                        .apply(color: Colors.grey)),
                trailing: item.success == null
                    ? Icon(Icons.more_horiz, color: Colors.grey)
                    : item.success == item.total
                        ? Icon(Icons.check, color: Colors.green)
                        : Icon(Icons.error, color: Colors.red),
              );
            }),
      ),
    );
  }
}
