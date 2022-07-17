try
{
    *(int*) 0 = 0;
}
catch (std::exception& e)
{
    std::cerr << "Exception caught : " << e.what() << std::endl;
}
