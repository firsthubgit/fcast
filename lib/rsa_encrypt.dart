import 'package:pointycastle/pointycastle.dart';
import 'package:asn1lib/asn1lib.dart';

import 'dart:convert';
import 'dart:typed_data';

const String kPublicKey = "-----BEGIN PUBLIC KEY-----\n"
    "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDFSgFlvSEXuj3KHntguiwtA/g9"
    "FzVjOcRuCiHSr4Vi4cHULaFHFHKfIbDSLTYj8GCocA25c3CGaX2nI32adxXiR+rq"
    "lZwMGb+jet3BS+sFMyOaAJ0DWJiJo4p8aaDiOmVH2eKIDqHB9GKRP2cw4V9o3Kie"
    "JZkFRWQIONBaInx0vwIDAQAB\n"
    "-----END PUBLIC KEY-----";

RSAPublicKey parsePublicKey(String pem) {
  final bytes = Uint8List.fromList(base64.decode(pem.split('\n')[1]));
  var parser = ASN1Parser(bytes);
  var seq = parser.nextObject() as ASN1Sequence;

  var bytes2 = seq.elements[1].valueBytes().sublist(1);
  var parser2 = ASN1Parser(Uint8List.fromList(bytes2));
  var seq2 = parser2.nextObject() as ASN1Sequence;

  final modulus = (seq2.elements[0] as ASN1Integer).valueAsBigInteger;
  final exponent = (seq2.elements[1] as ASN1Integer).valueAsBigInteger;
  // 
  // openssl rsa -pubin -inform PEM -text -noout < lib/corp-public.key
  // print('2 bigint\n$modulus\n$exponent');
  return RSAPublicKey(modulus, exponent);
}

String encryptWith(String publicKey, Map plain) {
  RSAPublicKey pk = parsePublicKey(publicKey);

  var cipher = AsymmetricBlockCipher('RSA/PKCS1');
  cipher.init(true, PublicKeyParameter<RSAPublicKey>(pk));

  Uint8List plainBuf = utf8.encode(json.encode(plain));
  Uint8List result = cipher.process(plainBuf);
  return base64.encode(result);
}

//
void main() {
  var map = <String, String>{
    'corpid': '1497349707',
    'timestamp': '1576477827',
  };
  String result = encryptWith(kPublicKey, map);
  print(result);
}
