import 'package:bloc/bloc.dart';

// Person 数据类定义
class Person extends Object {
  final String name;
  final int age;
  Person(this.name, this.age);

  @override
  String toString() {
    return 'Person{name: $name, age: $age}';
  }
}

// =========================== 使用 Cubit 的方案 =================================

class PersonCubit extends Cubit<Person> {
  // 构造函数
  PersonCubit(Person initialState) : super(initialState);

  // 监听数据发生改变, onChange 在数据发生改变前会被调用
  @override
  void onChange(Change<Person> change) {
    super.onChange(change);
    print('onChange: ${change.currentState} -> ${change.nextState}');
  }

  // 监听错误
  @override
  void onError(Object error, StackTrace stackTrace) {
    // print('onError: $error, $stackTrace');
    super.onError(error, stackTrace);
  }

  // 实现数据改变的方法, 提供外部调用
  void setAge(int age) {
    // 抛出错误
    if (age < 0) {
      addError(Exception('set age error!\n'), StackTrace.current);
      return ;
    }
    emit(Person(state.name, age));
  }

  void setName(String name) => emit(Person(name, state.age));
}

// =========================== 使用 Bloc 的方案 ==================================
// Bloc 方案外部调用需要传入一个事件对象

// 定义事件
sealed class PersonEvent {}
// 定义年龄改变时的事件
final class PersonAgeChanged extends PersonEvent {
  final int age;
  PersonAgeChanged(this.age);
}
// 定义名字改变时的事件
final class PersonNameChanged extends PersonEvent {
  final String name;
  PersonNameChanged(this.name);
}

class PersonBloc extends Bloc<PersonEvent, Person> {
  // 构造函数
  PersonBloc(Person initialState) : super(initialState) {
    // 注册事件处理器
    on<PersonAgeChanged>((event, emit) {
      // 抛出错误
      if (event.age < 0) {
        addError(Exception('set age error!\n'), StackTrace.current);
        return ;
      }
      emit(Person(state.name, event.age));
    });
    on<PersonNameChanged>((event, emit) {
      emit(Person(event.name, state.age));
    });
  }

  // Bloc 也继承自 BlocBase 所以也支持监听数据变化和错误
  @override
  void onError(Object error, StackTrace stackTrace) {
    super.onError(error, stackTrace);
  }

  @override
  void onChange(Change<Person> change) {
    super.onChange(change);
  }

  // onTransition 是 Bloc 独有的, 它能够捕获有关触发状态变化的信息
  // onTransition 在 onChange 之前被调用并且包含了触发 currentState 到 nextState的事件
  @override
  void onTransition(Transition<PersonEvent, Person> transition) {
    super.onTransition(transition);
    // print(transition);
  }

  // onEvent 也是 Bloc 独有的, 无论什么时候有新的事件被添加到 Bloc，这个方法都会被调用
  @override
  void onEvent(PersonEvent event) {
    super.onEvent(event);
    print(event);
  }
}

// =========== Bloc 观察器 (可同时观察 Cubit 和 Bloc 两种方案的状态变化) =============

class SimpleBlocObserver extends BlocObserver {

  @override
  void onChange(BlocBase bloc, Change change) {
    super.onChange(bloc, change);
    print('Observer (onChange): ${bloc.runtimeType}, $change');
  }

  @override
  void onError(BlocBase bloc, Object error, StackTrace stackTrace) {
    super.onError(bloc, error, stackTrace);
    print('Observer (onError): ${bloc.runtimeType}, $error, $stackTrace');
  }

  @override
  void onTransition(Bloc bloc, Transition transition) {
    super.onTransition(bloc, transition);
    print('Observer (onTransition): ${bloc.runtimeType}, $transition');
  }

  @override
  void onEvent(Bloc bloc, Object? event) {
    super.onEvent(bloc, event);
    print('Observer (onEvent): ${bloc.runtimeType}, $event');
  }
}

void main(List<String> arguments) async {

  // 注册 Bloc 观察器
  Bloc.observer = SimpleBlocObserver();
  
  print("\n=============== 使用 cubit ===================\n");
  final cubit = PersonCubit(Person('John', 30));
  // 监听数据变化
  final subscription = cubit.stream.listen((person) {
    print("listen change: ${person.name}, ${person.age}");
  });

  // 模拟用户操作, 传入新的属性值
  cubit.setAge(50);
  cubit.setName('Mary');
  print("${cubit.state.name}, ${cubit.state.age}");
  await Future.delayed(Duration.zero);

  print("\n=============== 使用 cubit 抛出错误 ===================\n");
  cubit.setAge(-10);

  await Future.delayed(Duration.zero);
  await subscription.cancel();
  await cubit.close();

  print("\n=============== 使用 bloc ===================\n");
  final bloc = PersonBloc(Person('John', 30));

  // 监听数据变化
  final subscription2 = bloc.stream.listen((person) {
    print("listen change: ${person.name}, ${person.age}");
  });
  // 模拟用户操作, 触发事件, 传入事件对象
  bloc.add(PersonAgeChanged(12));
  bloc.add(PersonNameChanged("Tom"));
  print("${bloc.state.name}, ${bloc.state.age}");
  await Future.delayed(Duration.zero);

  print("\n=============== 使用 bloc 抛出错误 ===================\n");
  bloc.add(PersonAgeChanged(-10));

  await Future.delayed(Duration.zero);
  await subscription2.cancel();
  await bloc.close();
}
