@echo Build starting...
@g++ example.cc -Og -static -o example 
@echo Build Completed!
@if not %errorlevel% equ 0 goto end
@echo ==========================================
@example
:end
