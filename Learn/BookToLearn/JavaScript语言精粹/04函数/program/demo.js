var quo = function (status) {

    var theobj = {
        status: "Mother",
        num: 123,
        get_status: function () {
            return this.status;
        }
    }

    return theobj;
};

// 构造一个 quo 实例
var q1 = quo("Q1");
var q2 = quo("Q2");

console.log(q1.num);
q2.num = 999;
console.log(q1.num);

console.log(q1.status);
