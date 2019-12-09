include .env

service:
	python model_checker.py --server ${THESIS_MODEL_CHECKER_PORT}

run:
	python model_checker.py --data-json "{\"ahref_link\":-1.0,\"asn\":1.0,\"dns_created\":0.9997473609825248,\"dns_updated\":0.9998847840872596,\"dnssec\":1.0,\"favicon_link\":-1.0,\"gtld\":1.0,\"host_length\":0.0,\"path_length\":0.19318181818181818,\"similar_domain\":0.3333333333333333,\"spec_chars_path\":0.14285714285714285,\"spec_chars_query\":0.0,\"spec_keywords\":0.0,\"src_link\":-1.0,\"url_length\":0.14965986394557823}"

pack:
	venv/bin/pyinstaller --clean --onefile --add-data pretrained_model.pkl:. --distpath bin --name model-checker model_checker.py

clean:
	rm -rf __pycache__ build bin dist *.spec
