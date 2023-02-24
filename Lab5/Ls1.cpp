#include <iostream>

using namespace std;

class int_ptr_wrapper{
    private:
        int *_ptr;
    protected:
        void set_val(int num){
            *_ptr = num;
        }
    public:
        int_ptr_wrapper(int num = 0){
            _ptr = new int();
            *_ptr = num;
        }
        int_ptr_wrapper(int_ptr_wrapper const& obj){
            _ptr = new int();
            *_ptr = *obj._ptr;
        }
        int_ptr_wrapper& operator=(int_ptr_wrapper const& obj){
            // delete this._ptr;
            // this._ptr = new int;
            *(this->_ptr) = *obj._ptr;
            return *this;
        }
        int_ptr_wrapper& operator+=(int_ptr_wrapper const& obj){
            *(this->_ptr) += *obj._ptr;
            return *this;
        }
        int_ptr_wrapper operator+(int_ptr_wrapper const& obj)const{
            return int_ptr_wrapper(*this) += obj;
        }
        
        int get_value()const{
            return *_ptr;   
        }
        
        friend ostream& operator<<(ostream& stream, const int_ptr_wrapper& obj){
            return stream << (*obj._ptr);
        }
        
        friend istream& operator>>(istream& stream, int_ptr_wrapper& obj){
            return stream >> (*obj._ptr);
        }
        
        ~int_ptr_wrapper(){
            delete _ptr;
        }
};

int main(){
    int_ptr_wrapper x(10);
    cout << "x is " << x << " at start" << endl;
    cin >> x;
    cout << "x is now " << x << endl;
    return 0;
}
