namespace __cxxabiv1 {
    class __class_type_info {
        virtual void dummy();
    };
    void __class_type_info::dummy() { }  // causes the vtable to get created here
};
