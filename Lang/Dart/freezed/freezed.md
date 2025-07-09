

## json_serializable

```dart
// 添加依赖
// dart pub add json_annotation
// dart pub add --dev json_serializable

// 代码生成
// dart run build_runner build --delete-conflicting-outputs
// 或者观察文件变化, 自动生成
// dart run build_runner watch --delete-conflicting-outputs

// main.dart
import 'dart:convert';
import 'model.dart';

void main(List<String> arguments) {
  // Example1. json to Object
  String jsonStr = ''' {
      "persons":[
        {"name":"张三", "hobby":"篮球", "other": "是积极分子"},
        {"name":"李四"}
      ], 
      "status": "success"
    } ''';

  Message? message;

  try {
    message = Message.fromJson(jsonDecode(jsonStr));
  } catch (e) {
    print("Error: $e");
  }

  if (message == null || message.persons == null) {
    return;
  }

  print("message.status => ${message.status}");
  for (var person in message.persons!) {
    print("person => name: ${person.name}, hobby: ${person.hobby}");
  }

  // Example2. Object to json
  message = Message(
    persons: [
      Person(name: "王五", hobby: "足球"),
      Person(name: "赵六", hobby: null),
    ],
    status: "success",
  );

  message.persons?.add(Person(name: "王七"));

  try {
    jsonStr = jsonEncode(message.toJson());
  } catch (e) {
    print("Error: $e");
  }

  print("jsonStr => $jsonStr");
}


// model.dart
import 'package:json_annotation/json_annotation.dart';

part 'model.g.dart';

@JsonSerializable()
class Person {
  String name;

  // hobby 如果为空, 则不要添加到 toJson 中
  @JsonKey(includeIfNull: false)
  String? hobby;

  Person({
    required this.name,
    this.hobby,
  });

  factory Person.fromJson(Map<String, dynamic> json) => _$PersonFromJson(json);

  Map<String, dynamic> toJson() => _$PersonToJson(this);
}


@JsonSerializable()
class Message {
  List<Person>? persons;

  @JsonKey(includeIfNull: false)
  String? status;

  Message({
    required this.persons,
    this.status,
  });

  factory Message.fromJson(Map<String, dynamic> json) => _$MessageFromJson(json);

  Map<String, dynamic> toJson() => _$MessageToJson(this);
}
```

## freezed

```dart
// 添加依赖
// dart pub add json_annotation
// dart pub add --dev json_serializable
// dart pub add --dev freezed
// dart pub add freezed_annotation

// 代码生成
// dart run build_runner build --delete-conflicting-outputs
// 或者观察文件变化, 自动生成
// dart run build_runner watch --delete-conflicting-outputs

// main.dart
import 'dart:convert';
import 'model.dart';

void main(List<String> arguments) {
  // Example1. json to Object
  String jsonStr = '''{
      "persons":[
        {"name":"张三", "hobby":"篮球", "other": "是积极分子"},
        {"name":"李四"}
      ], 
      "status": "success"
    }''';

  Message? message;

  try {
    message = Message.fromJson(jsonDecode(jsonStr));
  } catch (e) {
    print("Error: $e");
  }

  if (message == null) {
    return;
  }

  print("message.status => ${message.status}");
  for (var person in message.persons) {
    print("person => name: ${person.name}, hobby: ${person.hobby}");
  }

  // Example2. Object to json
  message = Message(
    persons: [
      Person(name: "王五", hobby: "足球"),
      Person(name: "赵六", hobby: null),
    ],
    status: "success",
  );

  // message.persons.add(Person(name: "王七"));
  final newPerson = Person(name: "王七");
  message = message.copyWith(persons: [...message.persons, newPerson]);


  try {
    jsonStr = jsonEncode(message.toJson());
  } catch (e) {
    print("Error: $e");
  }

  print("jsonStr => $jsonStr");
}


// model.dart
// ignore_for_file: invalid_annotation_target

import 'package:freezed_annotation/freezed_annotation.dart';

part 'model.g.dart';
part 'model.freezed.dart';

@freezed
abstract class Person with _$Person {

  const factory Person({
    required String name,
    @JsonKey(includeIfNull: false) String ?hobby,
  }) = _Person;

  factory Person.fromJson(Map<String, dynamic> json) => _$PersonFromJson(json);
}

@freezed
abstract class Message with _$Message {

  const factory Message({
    required List<Person> persons,
    @JsonKey(includeIfNull: false) String? status,
  }) = _Message;

  factory Message.fromJson(Map<String, dynamic> json) => _$MessageFromJson(json);
}
```


## freezed 多个构造方法

```dart
// 添加依赖
// dart pub add json_annotation
// dart pub add --dev json_serializable
// dart pub add --dev freezed
// dart pub add freezed_annotation

// 代码生成
// dart run build_runner build --delete-conflicting-outputs
// 或者观察文件变化, 自动生成
// dart run build_runner watch --delete-conflicting-outputs

// main.dart
import 'dart:convert';
import 'model.dart';

void main(List<String> arguments) {
  // Example1. json to Object
  String jsonStr = '''{
      "persons":[
        {
          "type": "normal",
          "name":"张三", 
          "hobby":"篮球", 
          "other": "是积极分子"
        },
        {
          "type": "student",
          "name":"李四",
          "school": "XX 中学"
        },
        {
          "type": "employee",
          "name":"王五",
          "company": "XX 公司"
        }
      ], 
      "status": "success"
    }''';


  Message? message;

  try {
    message = Message.fromJson(jsonDecode(jsonStr));
  } catch (e) {
    print("Error: $e");
  }

  if (message == null) {
    return;
  }

  print("message.status => ${message.status}");
  for (var person in message.persons) {
    print("person => name: ${person.name}, hobby: ${person.hobby}");
    person.map(
      normal: (normalPerson) {
        print('普通人员 ${normalPerson.name}，爱好是 ${normalPerson.hobby ?? '暂无'}');
      },
      student: (studentPerson) {
        print('学生 ${studentPerson.name}，就读于 ${studentPerson.school}，爱好是 ${studentPerson.hobby ?? '暂无'}');
      },
      employee: (employeePerson) {
        // 直接通过 . 操作符访问对象属性
        print('员工 ${employeePerson.name}，就职于 ${employeePerson.company}，爱好是 ${employeePerson.hobby ?? '暂无'}');
      },
    );
  }

  // Example2. Object to json
  message = Message(
    persons: [
      Person.normal(name: "王五", hobby: "足球"),
      Person.normal(name: "赵六", hobby: null),
    ],
    status: "success",
  );

  // message.persons.add(Person(name: "王七"));
  final newPerson = Person.normal(name: "王七");
  message = message.copyWith(persons: [...message.persons, newPerson]);


  try {
    jsonStr = jsonEncode(message.toJson());
  } catch (e) {
    print("Error: $e");
  }

  print("jsonStr => $jsonStr");
}


// model.dart
// ignore_for_file: invalid_annotation_target

import 'package:freezed_annotation/freezed_annotation.dart';

part 'model.g.dart';
part 'model.freezed.dart';

@Freezed(unionKey: 'type')
abstract class Person with _$Person {

  // 普通人员构造函数
  const factory Person.normal({
    required String name,
    @JsonKey(includeIfNull: false) String? hobby,
  }) = NormalPerson;

  // 学生构造函数，包含学校信息
  const factory Person.student({
    required String name,
    @JsonKey(includeIfNull: false) String? hobby,
    required String school,
  }) = StudentPerson;

  // 员工构造函数，包含公司信息
  const factory Person.employee({
    required String name,
    @JsonKey(includeIfNull: false) String? hobby,
    required String company,
  }) = EmployeePerson;

  factory Person.fromJson(Map<String, dynamic> json) => _$PersonFromJson(json);
}

@freezed
abstract class Message with _$Message {

  const factory Message({
    required List<Person> persons,
    @JsonKey(includeIfNull: false) String? status,
  }) = _Message;

  factory Message.fromJson(Map<String, dynamic> json) => _$MessageFromJson(json);
}
```
