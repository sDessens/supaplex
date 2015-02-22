#macro relocate(location)
 #ifdef g_old_location
   .error "you cannot nest relocate blocks"
 #else
   #define g_old_location eval($)
   #define g_new_location eval(location)
   .org location
 #endif
#endmacro

#macro endrelocate()
 #ifdef g_new_location 
   .org $ - g_new_location + g_old_location
   #undefine g_new_location 
   #undefine g_old_location
 #else
   .error "no relocate statements corresponds to this endrelocate"
 #endif
#endmacro
