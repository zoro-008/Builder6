//---------------------------------------------------------------------------

#ifndef LanguageUnitH
#define LanguageUnitH
//---------------------------------------------------------------------------
class CLangUnit
{
    private:

    public:
        CLangUnit(void);
        ~CLangUnit(void);

        void ReadFrmOperation   ();
        void ReadFrmDeviceSet   ();
        void ReadFrmOption      ();
        void ReadFrmSPC         ();
        void ReadFrmDevice      ();

        bool LoadFrmOperationHan();
        bool LoadFrmDeviceSetHan();
        bool LoadFrmOptionHan   ();
        bool LoadFrmSPCHan      ();
        bool LoadFrmDeviceHan   ();

};
extern CLangUnit LAG;
#endif
