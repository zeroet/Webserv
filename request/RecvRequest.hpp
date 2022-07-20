#ifndef REQUEST_HPP
# define REQUEST_HPP

# define CR '\r'
# define LF '\n'

# include <string>
# include <vector>
# include <map>

//recevie all until \r\n\r\n
//error
//1: timeout (timestamp)
//2: size >
class RecvRequest {
	public:
		RecvRequest(void);
		RecvRequest(int	client_fd);
		RecvRequest(RecvRequest const &src);
		RecvRequest &operator=(RecvRequest const &src);
		~RecvRequest(void);

	private:
		int			client_fd_;
		char		buf_;

	public:
		const int	&get_client_fd(void) const;
		char		&get_buf(void);
		void		set_buf(char buf);
};

#endif