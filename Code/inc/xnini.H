#ifndef _XN_INI_CONFIG_FILE_H_
#define _XN_INI_CONFIG_FILE_H_

struct xnIniFile {
	static xnIniFile * Open(char * iniFileName);
	void	Close(void);
	char *	ReadFristSection(void);
	char *	ReadNextSection(void);
	char *	ReadFristKey(void);
	char *	ReadNextKey(void);
	void	SetSection(char * section);
	char *	ReadString(char * key, char * defaultResult);
	int		ReadInt(char * key, int defaultResult);
	void	WriteInt(char * key, int writeVal);
	void	WriteString(char * key, char * format, ...);
};

#endif
