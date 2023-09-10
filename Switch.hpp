#ifndef __SWITCH_H__
#define __SWITCH_H__

enum class E_Switch_Status {
    On,
    Off
};

class Switch {
    private:
        int             pin_num;
        unsigned long   last_swtm = 0;
        E_Switch_Status sts = E_Switch_Status::Off;
    public:
        Switch (int);
        void interrupt (int);
        void setPressEvent (void);
        void setLongPressEvent (void);
        void chkLoop(void);
        int  get_pin (void);
};



#endif
