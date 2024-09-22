namespace drug_lib::common::db
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