/**
 * @file CallbackM.h
 * @brief Application callback manager interface.
 * @details Initializes application callback registrations without exposing
 *          the underlying hardware or CDD implementation.
 * @req AUTOSAR service-layer interface.
 */
#ifndef SV_MANAGER_H
#define SV_MANAGER_H
void Serice_Callback_Init(void);

#endif