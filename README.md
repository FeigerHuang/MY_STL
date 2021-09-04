## MY-STL is a project that created by feiger 
- in order to study data structure and template technology from excellent engineers!
- 本项目参考资料 << STL-源码刨析>>--侯捷
- 参考资料  SGI-STL v3.3   
- 项目纯粹用于个人学习, 如发现纰漏欢迎指导!
(〃￣︶￣)人(￣︶￣〃)🍖🍖🍖🍖🍖
> **臣之所好者，道也，进乎技矣。始臣之解牛之时，所见无非牛者.
三年之后，未尝见全牛也。方今之时，臣以神遇而不以目视，
官知止而神欲行。依乎天理，批大郤，导大窾，因其固然，技经肯綮之未尝，而况大軱乎！
良庖岁更刀，割也；族庖月更刀，折也。今臣之刀十九年矣，所解数千牛矣，而刀刃若新发于硎。
彼节者有间，而刀刃者无厚；以无厚入有间，恢恢乎其于游刃必有余地矣，是以十九年而刀刃若新发于硎。
虽然，每至于族，吾见其难为，怵然为戒，视为止，行为迟。
动刀甚微，謋然已解，如土委地。提刀而立，为之四顾，为之踌躇满志，善刀而藏之。--<<庖丁解牛>>**

-  借庖丁之言与诸君共勉!

------------------------------------------------------------------
- 目前项目实现包括 :   容器 : vector, deque, list, set, map, slist, hashtable...
                       仿函数 : greater, less, logic_and, logic_or....
                       算法 : sort, lower_bound, up_bound, inner_product, heap算法.....
                       空间配置器 : 一级空间配置器 allocator, 二级空间配置器 super_allocator;
                       容器适配器 : stack, queue, priority_que..
                       各容器迭代器 及 相关萃取技术;
- 目前不足之处 : 空间配置器及相关容器不支持多线程, 未使用lock, 部分容器未实现移动拷贝, 移动赋值运算符重载;
                 构造函数较为单一, 未实现列表初始化构造与传入迭代器构造; 
