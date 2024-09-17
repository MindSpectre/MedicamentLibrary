namespace ControlSystemV7::Common::Services::Exceptions
{
    namespace ReturnErrorCodes
    {
        enum ReturnErrorCodes
        {
            OK = 100,
            // SECURITY
            InvalidHash = 201,
            // service errors
            InvalidDataReceiven = 211,
            InvalidProductVolume = 212,
            InvalidProductId = 213,

            ///  SERVER ERROR
            // DbError
            DbExceptionNoError = 610,
            DbExceptionCreateTableError = 611,
            DbExceptionRemoveTableError = 612,
            DbExceptionHasTableError = 613,
            DbExceptionAddRowError = 614,
            DbExceptionQueryError = 615,
            DbExceptionIncorrectNumberOfRecordsError = 616,
            DbExceptionUnknownError = 617,

            // InaccesibleSourceFiles
            InaccesibleJsonSchema = 620,
            UnknownError = 900
        };
    }
}
/*
| Value     | Description    |
| :---:     | :---:          |
| 100       | OK.No errors   |
| 211       | Invalid hash   |
| 221       | Invalid data   |
| 222       | Invalid volume of fuel/good |
| 223       | Invalid product. |
| 310       | DB Exception:NoError  |
| 311       | DB Exception:CreateTableError  |
| 312       | DB Exception:RemoveTableError  |
| 313       | DB Exception:HasTableError  |
| 314       | DB Exception:AddRowError  |
| 315       | DB Exception:QueryError  |
| 316       | DB Exception:UnknownError  |
| 330       | Inaccessible files. |
| 900       | Unknown error |
*/
